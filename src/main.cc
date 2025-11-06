#include "internal.hh"
#include "api.hh"

int main() {
    std::cout << "Hello world\n";

    save_state new_state("staffsave.ls");

    new_state.save();

    save_state load_state("staffsave.ls");

    print_state(load_state);

    distribution_export distr = generate_distribution_export(load_state);
    std::cout << print_distribution_export(distr).str();

    return 0;
}

#include "internal.hh"
#include "api.hh"
#include <iostream>

// written with chatgpt just to generate quick staff and student names
// int main() {
//     save_state new_state("staffsave.ls");

//     t_name_id alice = new_state.append_name("Alice");
//     t_name_id bob   = new_state.append_name("Bob");
//     t_name_id carol = new_state.append_name("Carol");
//     t_name_id dave  = new_state.append_name("Dave");

//     new_state.append_staff(staff_entry(alice, f_lunch_availability::AA | f_lunch_availability::AB | f_lunch_availability::BA));
//     new_state.append_staff(staff_entry(bob,   f_lunch_availability::AB | f_lunch_availability::BB));
//     new_state.append_staff(staff_entry(carol, f_lunch_availability::AC | f_lunch_availability::BC));
//     new_state.append_staff(staff_entry(dave,  f_lunch_availability::AA | f_lunch_availability::BB | f_lunch_availability::BC));

//     t_name_id s1 = new_state.append_name("Student1");
//     t_name_id s2 = new_state.append_name("Student2");
//     t_name_id s3 = new_state.append_name("Student3");
//     t_name_id s4 = new_state.append_name("Student4");
//     t_name_id s5 = new_state.append_name("Student5");

//     new_state.append_student(student_entry(s1, e_lunch_period::A, e_lunch_period::B));
//     new_state.append_student(student_entry(s2, e_lunch_period::B, e_lunch_period::C));
//     new_state.append_student(student_entry(s3, e_lunch_period::C, e_lunch_period::A));
//     new_state.append_student(student_entry(s4, e_lunch_period::A, e_lunch_period::C));
//     new_state.append_student(student_entry(s5, e_lunch_period::B, e_lunch_period::B));

//     if (!new_state.save()) {
//         std::cout << "Failed to save state\n";
//         return 1;
//     }

//     save_state load_state("staffsave.ls");

//     std::cout << print_state(load_state).str();

//     distribution_export distr = generate_distribution_export(load_state);

//     std::cout << print_distribution_export(distr).str();

//     return 0;
// }
