#pragma once

#include "internal.hh"

/*

Strategy:

Iterate through all students, check what lunches they are in, create period distributions based on that information.

*/

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

// Generate a complete distribution of where teachers should be in a given time based on their availability and what lunches the students are in.
distribution_export generate_distribution_export(const save_state& state);

std::stringstream print_distribution_export(const distribution_export& distr);
std::stringstream print_state(const save_state& state);
std::stringstream print_debug_state(const save_state& state);