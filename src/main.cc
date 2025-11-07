#include <iostream>
#include <limits>

#include "api.hh"

#define PROD_MODE true

#if PROD_MODE

int in_int() {
    int in;
    std::cin >> in;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return in;
}

std::string in_string() {
    std::string str;
    std::getline(std::cin, str);

    return str;
}

int main() {
    staff_session active_session("staffsave.ls");

    while (true) {
        system("cls");

        std::cout << active_session.print_state().str();
    }

    return 0;
}
#else
// written with chatgpt just to generate quick staff and student names
int main() {
    std::cout << "append mode\n";

    save_state new_state("staffsave.ls");

    t_name_id alice = new_state.add_or_find_name("Alice");
    t_name_id bob   = new_state.add_or_find_name("Bob");
    t_name_id carol = new_state.add_or_find_name("Carol");
    t_name_id dave  = new_state.add_or_find_name("Dave");

    new_state.append_staff(staff_entry(alice, f_period_index::AA | f_period_index::AB | f_period_index::BA, 2));
    new_state.append_staff(staff_entry(bob,   f_period_index::AB | f_period_index::BB, 2));
    new_state.append_staff(staff_entry(carol, f_period_index::AC | f_period_index::BC, 2));
    new_state.append_staff(staff_entry(dave,  f_period_index::AA | f_period_index::BB | f_period_index::BC, 1));

    t_name_id s1 = new_state.add_or_find_name("Student1");
    t_name_id s2 = new_state.add_or_find_name("Student2");
    t_name_id s3 = new_state.add_or_find_name("Student3");
    t_name_id s4 = new_state.add_or_find_name("Student4");
    t_name_id s5 = new_state.add_or_find_name("Student5");

    new_state.append_student(student_entry(s1, e_lunch_period::A, e_lunch_period::B));
    new_state.append_student(student_entry(s2, e_lunch_period::B, e_lunch_period::A));
    new_state.append_student(student_entry(s3, e_lunch_period::C, e_lunch_period::A));
    new_state.append_student(student_entry(s4, e_lunch_period::A, e_lunch_period::A));
    new_state.append_student(student_entry(s5, e_lunch_period::A, e_lunch_period::B));

    if (!new_state.save()) {
        std::cout << "Failed to save state\n";
        return 1;
    }

    save_state load_state("staffsave.ls");

    std::cout << print_state(load_state).str();

    distribution_export distr = generate_distribution_export(load_state);

    std::cout << print_distribution_export(distr).str();

    return 0;
}
#endif