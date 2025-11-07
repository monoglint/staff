#pragma once

#include <unordered_set>
#include <iterator>

#include "internal.hh"

// CAN NOT BE HIGHER THAN 256 - PROGRAM RUNS ON 8 BIT NUMBERS MAINLY
const uint8_t MAX_ENTRIES = 250;

struct period_selection {
    period_selection() {}
    period_selection(t_name_id_list&& staff_list, t_name_id_list&& student_list)
        : staff_list(std::move(staff_list)), student_list(std::move(student_list)) {}

    t_name_id_list staff_list;
    t_name_id_list student_list;
};

using t_period_selection_list = std::vector<period_selection>;

// For exporting
struct distribution_export {
    t_name_list name_list;
    t_period_selection_list period_selection_list;

    // Whether all periods with students have at least one teacher.
    bool are_all_periods_covered;
};

// The primary interface struct you should use to work with the program.
// Follows RAII
struct staff_session {
    staff_session(const std::string& file_path)
        : state(file_path) {}

    save_state state;

    distribution_export generate_distribution_export() const;

    std::stringstream print_distribution_export(const distribution_export& distr) const;
    std::stringstream print_distribution_export() const;
    std::stringstream print_state() const;
    std::stringstream print_debug_state() const;

    enum class e_add_staff_code {
        SUCCESS,
        EXCEEDED_MAX,
    };

    inline e_add_staff_code add_staff(const std::string& name, const uint8_t max_lunches, const std::unordered_set<e_period_index>& available_periods) {
        if (state.name_list.size() >= MAX_ENTRIES || state.staff_list.size() >= MAX_ENTRIES)
            return e_add_staff_code::EXCEEDED_MAX;
        
        f_period_index availability = f_period_index::NONE;

        for (const e_period_index period_index : available_periods) {
            availability = availability | enum_to_flag<e_period_index, f_period_index>(period_index);
        }

        state.append_staff(
            staff_entry(
                state.add_or_find_name(name),
                availability,
                max_lunches
            )
        );

        return e_add_staff_code::SUCCESS;
    }

    enum class e_add_student_code {
        SUCCESS,
        EXCEEDED_MAX,
    };

    inline e_add_student_code add_student(const std::string& name, const e_lunch_period a_day_lunch, const e_lunch_period b_day_lunch) {
        if (state.name_list.size() >= MAX_ENTRIES || state.student_list.size() >= MAX_ENTRIES)
            return e_add_student_code::EXCEEDED_MAX;

        state.append_student(
            student_entry(
                state.add_or_find_name(name),
                a_day_lunch,
                b_day_lunch
            )
        );

        return e_add_student_code::SUCCESS;
    }

    inline void edit_staff(const std::string& name, const std::unordered_set<e_period_index> available_periods) {
        std::cout << "NOT IMPLEMENTED\n";
    }

    inline void edit_student(const std::string& name, const e_lunch_period a_day_lunch, const e_lunch_period b_day_lunch) {
        std::cout << "NOT IMPLEMENTED\n";
    }

    enum class e_remove_code {
        SUCCESS,
        NO_MATCHING_NAME,
    };

    // Erase any entries that match the given name.
    inline e_remove_code remove(const std::string& name) {
        const t_name_list::iterator& name_id_iterator = std::find(state.name_list.begin(), state.name_list.end(), name);

        if (name_id_iterator == state.name_list.end())
            return e_remove_code::NO_MATCHING_NAME;

        const t_name_id name_id = std::distance(state.name_list.begin(), name_id_iterator);

        for (t_staff_list::iterator it = state.staff_list.begin(); it != state.staff_list.end();) {
            if (it->name_id == name_id)
                it = state.staff_list.erase(it);
            else
                it++;
        }

        // boilerplate - try to remove if possible
        for (t_student_list::iterator it = state.student_list.begin(); it != state.student_list.end();) {
            if (it->name_id == name_id)
                it = state.student_list.erase(it);
            else
                it++;
        }

        return e_remove_code::SUCCESS;
    }
};