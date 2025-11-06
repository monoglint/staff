#include "api.hh"

// Number of lunches per day. Used in generate_distribution_export
const uint8_t ADD_CONSTANT = 3;

using t_period_index_base = size_t;
// Used just for keeping track of period distributions.
enum class e_period_index : t_period_index_base {
    AA = 0, // A
    AB = 1, // B
    AC = 2, // C
    BA = 3, // A + ADD_CONSTANT
    BB = 4, // B + ADD_CONSTANT
    BC = 5, // C + ADD_CONSTANT
    MAX = 6,
};

using t_period_index_list = std::vector<e_period_index>;

static inline void append_student_to_export(distribution_export& distr, t_period_index_list& periods_with_students_list, t_name_id student_name_id, e_period_index period_index) {
    distr.period_selection_list[(t_period_index_base)period_index].student_list.push_back(student_name_id);
    
    if (std::find(periods_with_students_list.begin(), periods_with_students_list.end(), period_index) == periods_with_students_list.end())
        periods_with_students_list.push_back(period_index);
}

distribution_export generate_distribution_export(const save_state& state) {
    distribution_export distr;

    distr.name_list = state.name_list;
    distr.period_selection_list.resize((size_t)e_period_index::MAX);

    // Keep track of the list of periods that have students in them.
    t_period_index_list periods_with_students_list;

    for (t_entry_id student_id = 0; student_id < state.student_list.size(); student_id++) {
        const student_entry& entry = state.student_list[student_id];

        append_student_to_export(distr, periods_with_students_list, entry.name_id, (e_period_index)entry.a_day_lunch);
        append_student_to_export(distr, periods_with_students_list, entry.name_id, (e_period_index)((t_period_index_base)entry.b_day_lunch + 3));
    }

    for (e_period_index period : periods_with_students_list) {
        for (const staff_entry& staff : state.staff_list) {
            if (!flag_match(staff.availability, enum_to_flag<e_period_index, f_lunch_availability>(period)))
                continue;
            
            distr.period_selection_list[(size_t)period].staff_list.push_back(staff.name_id);
        }
    }

    // transparency note: minor ai help with discovery of std::all_of
    distr.are_all_periods_covered = std::all_of(
        periods_with_students_list.begin(),
        periods_with_students_list.end(),
        [&](e_period_index period) {
            const period_selection& slot = distr.period_selection_list[(size_t)period];
            return !slot.student_list.empty() ? !slot.staff_list.empty() : true;
        }
    );

    return distr;
}

std::stringstream print_distribution_export(const distribution_export& distr) {
    std::stringstream buffer;
    
    buffer << "A/B schedule\n";

    if (!distr.are_all_periods_covered)
        buffer << "// WARNING - NOT ENOUGH STAFF TO FILL LUNCHES \\\\";
    
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
        for (const t_name_id name_id : period.staff_list) {
            buffer << "\t\t\t" << distr.name_list[name_id] << '\n';
        }

        buffer << "\t\tStudents:\n";
        for (const t_name_id name_id : period.student_list) {
            buffer << "\t\t\t" << distr.name_list[name_id] << '\n';
        }
    }

    return buffer;
}

std::stringstream print_state(const save_state& state) {
    std::stringstream buffer;
    
    buffer << "Staff List:\n";
    for (const staff_entry& entry : state.staff_list) { 
        buffer << "    Name:         " << state.name_list[entry.name_id] << '\n';
        buffer << "    Availability: " << to_string(entry.availability) << '\n';
        buffer << '\n';
    }

    buffer << "Student List:\n";
    for (const student_entry& entry : state.student_list) { 
        buffer << "    Name:             " << state.name_list[entry.name_id] << '\n';
        buffer << "    A Day Lunch:      " << to_string(entry.a_day_lunch) << '\n';
        buffer << "    B Day Lunch:      " << to_string(entry.b_day_lunch) << '\n';
        buffer << '\n';
    }

    return buffer;
}

std::stringstream print_debug_state(const save_state& state) {
    std::stringstream buffer;

    buffer << "NAMES:\n";
    for (const std::string& identifier : state.name_list) { 
        buffer << "NAME: " << identifier << '\n';
    }

    buffer << print_state(state).str();

    return buffer;
}