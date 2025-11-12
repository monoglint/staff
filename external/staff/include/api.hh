#pragma once

#include <unordered_set>
#include <iterator>
#include <iostream>

#include "internal.hh"

// CAN NOT BE HIGHER THAN 256 - PROGRAM RUNS ON 8 BIT NUMBERS MAINLY
const uint8_t MAX_ENTRIES = 250;

struct period_selection {
    period_selection() {}
    period_selection(t_name_list&& staff_list, t_name_list&& student_list)
        : staff_list(std::move(staff_list)), student_list(std::move(student_list)) {}

    t_name_list staff_list;
    t_name_list student_list;
};

using t_period_selection_list = std::vector<period_selection>;

// For exporting
struct distribution_export {
    t_period_selection_list period_selection_list;

    // Whether all periods with students have at least one teacher.
    bool are_all_periods_covered;
};

enum class e_entry_type {
    STAFF,
    STUDENT
};

// The primary interface struct you should use to work with the program.
// Follows RAII
struct staff_session {
    staff_session(const std::string& file_path)
        : state(file_path) {}

    save_state state;

    void export_csv(const std::string& file_path) const;

    distribution_export generate_distribution_export() const;

    std::stringstream print_distribution_export(const distribution_export& distr) const;
    std::stringstream print_distribution_export() const;
    std::stringstream print_state() const;

    inline t_entry_id add_staff(const std::string& name, const f_period_index availability) {
        state.staff_list.emplace_back(
            name,
            availability
        );

        return state.staff_list.size() - 1;
    }

    inline void remove_staff(const t_entry_id id) {
        state.staff_list.erase(state.staff_list.begin() + id);
    }

    inline t_entry_id add_student(const std::string& name, const e_lunch_period a_day_lunch, const e_lunch_period b_day_lunch) {
        state.student_list.emplace_back(
            name,
            a_day_lunch,
            b_day_lunch
        );

        return state.student_list.size() - 1;
    }

    inline void remove_student(const t_entry_id id) {
        state.student_list.erase(state.student_list.begin() + id);
    }

    inline staff_entry& get_staff(const t_entry_id id) {
        return state.staff_list[id];
    }

    inline student_entry& get_student(const t_entry_id id) {
        return state.student_list[id];
    }
};