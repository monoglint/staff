#include <optional>
#include <fstream>

#include "api.hh"

static inline void append_student_to_export(distribution_export& distr, t_period_index_list& periods_with_students_list, const std::string& student_name, const e_period_index period_index) {
    distr.period_selection_list[(t_period_index_base)period_index].student_list.push_back(student_name);
    
    if (std::find(periods_with_students_list.begin(), periods_with_students_list.end(), period_index) == periods_with_students_list.end())
        periods_with_students_list.push_back(period_index);
}

static inline bool is_period_schedulable(const t_name_list& selected_staff_list, const f_period_index period_as_flag, const staff_entry& staff_entry) {
    return flag_match(staff_entry.availability, period_as_flag) && std::find(selected_staff_list.begin(), selected_staff_list.end(), staff_entry.name) == selected_staff_list.end();
}

static inline bool is_a_day_period(e_period_index period) {
    return (t_period_index_base)period < 3;
}

static inline int count_assignments_on_a_day(const distribution_export& distr, const std::string& staff_name) {
    int count = 0;
    for (int period_idx = 0; period_idx < 3; ++period_idx) {
        const auto& staff_list = distr.period_selection_list[period_idx].staff_list;
        if (std::find(staff_list.begin(), staff_list.end(), staff_name) != staff_list.end()) {
            count++;
        }
    }
    return count;
}

static inline int count_assignments_on_b_day(const distribution_export& distr, const std::string& staff_name) {
    int count = 0;
    for (int period_idx = 3; period_idx < 6; ++period_idx) {
        const auto& staff_list = distr.period_selection_list[period_idx].staff_list;
        if (std::find(staff_list.begin(), staff_list.end(), staff_name) != staff_list.end()) {
            count++;
        }
    }
    return count;
}

/*

This function makes an initial pass on the staff list and attempts to fill up the periods as
evenly as possible. Staff can be assigned up to 2 lunch blocks per day (A-day or B-day).

*/

/*

TRANSPARENCY NOTE
    Although most of these functions were designed by me, AI was used to revise some minor aspects to work with
    more complicated constraints.

*/

void fill_staff_pass1(const save_state& state, distribution_export& distr, t_period_index_list& periods_with_students_list) {
    t_staff_list staff_list_copy = state.staff_list;

    while (staff_list_copy.size() > 0) {
        size_t staff_list_size = staff_list_copy.size();

        // Prioritize periods with less staff.
        std::sort(periods_with_students_list.begin(), periods_with_students_list.end(), [&](const e_period_index a, const e_period_index b) {
            return distr.period_selection_list[(t_period_index_base)a].staff_list.size() < distr.period_selection_list[(t_period_index_base)b].staff_list.size();
        });

        for (const e_period_index period : periods_with_students_list) {
            const f_period_index period_as_flag = enum_to_flag<e_period_index, f_period_index>(period);
            t_name_list& selected_staff_list = distr.period_selection_list[(size_t)period].staff_list;

            for (t_staff_list::iterator it = staff_list_copy.begin(); it != staff_list_copy.end();) {
                // If the staff isn't available or already is scheduled in the given lunch block.
                if (!is_period_schedulable(selected_staff_list, period_as_flag, *it)) {
                    ++it;
                    continue;
                }

                // Check if staff is already at 2 assignments on this day
                bool is_a_day = is_a_day_period(period);
                int assignments_on_day = is_a_day 
                    ? count_assignments_on_a_day(distr, it->name) 
                    : count_assignments_on_b_day(distr, it->name);

                if (assignments_on_day >= 2) {
                    // Already has 2 slots on this day, skip
                    ++it;
                    continue;
                }

                selected_staff_list.push_back(it->name);

                // Check if staff now has 2 on both A and B days; if so, fully remove them
                int a_day_count = count_assignments_on_a_day(distr, it->name);
                int b_day_count = count_assignments_on_b_day(distr, it->name);

                if (a_day_count >= 2 && b_day_count >= 2) {
                    // Staff is now scheduled for all their slots (2 per day), remove from pool
                    it = staff_list_copy.erase(it);
                } else {
                    // Keep them in pool for more assignments (up to 2 per day)
                    ++it;
                }

                break;
            }
        }

        // No more available edits can be made.
        if (staff_list_copy.size() == staff_list_size)
            break;
    }
}

void fill_staff_pass2(const save_state& state, distribution_export& distr, t_period_index_list& periods_with_students_list) {
    for (const e_period_index target_period_index : periods_with_students_list) {
        period_selection& target_period = distr.period_selection_list[(size_t)target_period_index];

        if (!target_period.staff_list.empty() || target_period.student_list.empty())
            continue;

        const f_period_index target_flag = enum_to_flag<e_period_index, f_period_index>(target_period_index);

        for (const e_period_index donor_period_index : periods_with_students_list) {
            if (donor_period_index == target_period_index) continue;
            auto& donor_period = distr.period_selection_list[(size_t)donor_period_index];

            // "magic number" - ensure that any donor period already has multiple staff
            if (donor_period.staff_list.size() < 2) continue;

            // try to move one staff member who's available for the target
            for (t_name_list::iterator it = donor_period.staff_list.begin(); it != donor_period.staff_list.end(); ++it) {
                const std::string& name = *it;

                const auto& staff_it = std::find_if(
                    state.staff_list.begin(), state.staff_list.end(),
                    [&](const staff_entry& s){ return s.name == name; }
                );
                if (staff_it == state.staff_list.end()) continue;

                // Must be available for the target period
                if (!flag_match(staff_it->availability, target_flag)) continue;

                // Ensure moving this staff member won't exceed the per-day limit (2 per day)
                bool target_is_a_day = is_a_day_period(target_period_index);
                int assignments_on_target_day = target_is_a_day
                    ? count_assignments_on_a_day(distr, name)
                    : count_assignments_on_b_day(distr, name);

                if (assignments_on_target_day >= 2) {
                    // This staff already has 2 assignments on the target day; skip
                    continue;
                }

                // Safe to move
                target_period.staff_list.push_back(name);
                donor_period.staff_list.erase(it);
                goto next_period; // move to next empty period
            }
        }
        next_period:;
    }
}

/* 
    The final pass erases extra staff members until there is on left in each block

    Later refactor so staff with a larger number of spots in other lunches get removed first.
*/
void fill_staff_pass3(const save_state& state, distribution_export& distr, t_period_index_list& periods_with_students_list) {
    for (const e_period_index target_period_index : periods_with_students_list) {
        period_selection& target_period = distr.period_selection_list[(size_t)target_period_index];
        
        if (target_period.staff_list.size() <= 1)
            continue;
        target_period.staff_list.resize(1);
        target_period.staff_list.shrink_to_fit();
    }
}

distribution_export staff_session::generate_distribution_export() const {
    distribution_export distr;

    distr.period_selection_list.resize((size_t)e_period_index::_MAX);

    // Keep track of the list of periods that have students in them.
    t_period_index_list periods_with_students_list;

    for (t_entry_id student_id = 0; student_id < state.student_list.size(); student_id++) {
        const student_entry& entry = state.student_list[student_id];

        append_student_to_export(distr, periods_with_students_list, entry.name, (e_period_index)entry.a_day_lunch);
        append_student_to_export(
            distr,
            periods_with_students_list,
            entry.name,
            (e_period_index)((t_period_index_base)entry.b_day_lunch + ADD_CONSTANT)
        );
    }

    fill_staff_pass1(state, distr, periods_with_students_list);
    fill_staff_pass2(state, distr, periods_with_students_list);
    fill_staff_pass3(state, distr, periods_with_students_list);

    distr.are_all_periods_covered = std::all_of(
        distr.period_selection_list.begin(),
        distr.period_selection_list.end(),
        [&](const period_selection& slot) {
            return !slot.student_list.empty() ? !slot.staff_list.empty() : true;
        }
    );

    return distr;
}

void staff_session::export_csv(const std::string& file_path) const {
    std::ofstream out(file_path);

    if (!out.is_open()) {
        return;
    }

    const distribution_export distr = generate_distribution_export();

    out << "Lunch,Staff,Students\n";
    for (uint8_t period_selection_list_index = 0; period_selection_list_index < distr.period_selection_list.size(); period_selection_list_index++) {
        std::string header;
        switch (period_selection_list_index) {
            case 0:
                header = "A Day - A";
                break;
            case 1:
                header = "A Day - B";
                break;
            case 2:
                header = "A Day - C";
                break;
            case 3:
                header = "B Day - A";
                break;
            case 4:
                header = "B Day - B";
                break;
            case 5:
                header = "B Day - C";
                break;
        }

        const period_selection& period = distr.period_selection_list[period_selection_list_index];
        const std::string staff_name = (period.staff_list.size() > 0) ? period.staff_list.at(0) : "[None]";

        out << header << "," << staff_name << ",";
        for (const std::string& student_name : period.student_list) {
            out << student_name << ",";
        }
        out << "\n";
    }
}

std::stringstream staff_session::print_distribution_export() const {
    return print_distribution_export(generate_distribution_export());
}

std::stringstream staff_session::print_distribution_export(const distribution_export& distr) const {
    std::stringstream buffer;
    
    buffer << "A/B schedule\n";

    if (!distr.are_all_periods_covered)
        buffer << "// WARNING - NOT ENOUGH STAFF TO FILL LUNCHES \\\\\n";
    
    for (uint8_t period_selection_list_index = 0; period_selection_list_index < distr.period_selection_list.size(); period_selection_list_index++) {
        switch (period_selection_list_index) {
            case 0:
                buffer << "A Day:\n";
                buffer << "\tA Lunch:\n";
                break;
            case 1:
                buffer << "\tB Lunch:\n";
                break;
            case 2:
                buffer << "\tC Lunch:\n";
                break;
            case 3:
                buffer << "B Day:\n";
                buffer << "\tA Lunch:\n";
                break;
            case 4:
                buffer << "\tB Lunch:\n";
                break;
            case 5:
                buffer << "\tC Lunch:\n";
                break;
        }

        const period_selection& period = distr.period_selection_list[period_selection_list_index];
        
        buffer << "\t\tStaff:\n";
        for (const std::string& name : period.staff_list) {
            buffer << "\t\t\t" << name << '\n';
        }

        buffer << "\t\tStudents:\n";
        for (const std::string& name : period.student_list) {
            buffer << "\t\t\t" << name << '\n';
        }
    }

    return buffer;
}

std::stringstream staff_session::print_state() const {
    std::stringstream buffer;
    
    buffer << "Staff List:\n";
    for (const staff_entry& entry : state.staff_list) { 
        buffer << "    Name:            " << entry.name << '\n';
        buffer << "    Availability:    " << to_string(entry.availability) << '\n';
        buffer << '\n';
    }

    buffer << "Student List:\n";
    for (const student_entry& entry : state.student_list) { 
        buffer << "    Name:             " << entry.name << '\n';
        buffer << "    A Day Lunch:      " << to_string(entry.a_day_lunch) << '\n';
        buffer << "    B Day Lunch:      " << to_string(entry.b_day_lunch) << '\n';
        buffer << '\n';
    }

    return buffer;
}