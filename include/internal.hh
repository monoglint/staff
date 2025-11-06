/*

savestate.hh

Updated by Jonah Higgins
4 Nov 2025

Holds the data structures for storing information about staff and student entries in a (somewhat)
efficient way.

app.hh currently carries all of the functions meant for interracting with the datastructure in
an abstract way.

*/

#pragma once

#include <fstream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cstdint>

#include "core.hh"

constexpr uint8_t TOK_DELIM = '\x1e'; // this is not a token, it delimits them

enum class e_lunch_period : uint8_t {
    A,
    B,
    C
};

enum class f_lunch_availability : uint8_t {
    NONE = 0,
    AA = 1 << 0,
    AB = 1 << 1,
    AC = 1 << 2,
    BA = 1 << 3,
    BB = 1 << 4,
    BC = 1 << 5,
};

inline f_lunch_availability operator|(f_lunch_availability a, f_lunch_availability b) {
    return (f_lunch_availability)((uint8_t)a | (uint8_t)b);
}

inline f_lunch_availability operator&(f_lunch_availability a, f_lunch_availability b) {
    return (f_lunch_availability)((uint8_t)a & (uint8_t)b);
}

using t_name_id = uint8_t; // index in t_name_list
using t_name_list = std::vector<std::string>;
using t_name_id_list = std::vector<t_name_id>;

using t_serialized_entry = uint32_t;
using t_serialized_entry_list = std::vector<t_serialized_entry>;

#define FM_B(la, str) if (flag_match(availability, f_lunch_availability::la)) buffer << str;
inline std::string to_string(const f_lunch_availability& availability) {
    std::stringstream buffer;

    FM_B(AA, "AA ")
    FM_B(AB, "AB ")
    FM_B(AC, "AC ")
    FM_B(BA, "BA ")
    FM_B(BB, "BB ")
    FM_B(BC, "BC ")

    if (buffer.str().length() == 0)
        buffer << "NO AVAILABILITY\n";

    return buffer.str();
}
#undef FM_B

inline std::string to_string(const e_lunch_period& period) {
    switch (period) {
        case e_lunch_period::A:
            return "A";
        case e_lunch_period::B:
            return "B";
        case e_lunch_period::C:
            return "C";
        default:
            return "INVALID";
    }
}

struct staff_entry {
    staff_entry(const t_name_id name_id, const f_lunch_availability availability = f_lunch_availability::NONE)
        : name_id(name_id), availability(availability) {}

    t_name_id name_id;
    f_lunch_availability availability;
};

struct student_entry {
    student_entry(const t_name_id name_id, const e_lunch_period a_day_lunch, const e_lunch_period b_day_lunch)
        : name_id(name_id), a_day_lunch(a_day_lunch), b_day_lunch(b_day_lunch) {}

    t_name_id name_id; // 8

    e_lunch_period a_day_lunch; // 8
    e_lunch_period b_day_lunch; // 8
};

using t_staff_list = std::vector<staff_entry>;
using t_student_list = std::vector<student_entry>;

using t_entry_id = size_t;
using t_entry_id_list = std::vector<t_entry_id>;

struct save_state {
    save_state(const std::string& file_name, const t_staff_list& staff_list, const t_student_list& student_list)
        : file_name(file_name), staff_list(staff_list), student_list(student_list) {}

    save_state(const std::string& file_name)
        : file_name(file_name) {
        const bool success = load();

        if (!success) {
            std::cout << "Failed to open \"" + file_name + "\".\n";
        }
    }

    std::string file_name;

    t_name_list name_list;
    t_staff_list staff_list;
    t_student_list student_list;

    inline t_name_id append_name(const std::string& name) {
        const auto& iterator = std::find(name_list.begin(), name_list.end(), name);
        if (iterator != name_list.end())
            return std::distance(name_list.begin(), iterator);

        name_list.push_back(name);
        return name_list.size() - 1;
    }

    inline t_entry_id append_staff(staff_entry&& entry) {
        staff_list.push_back(std::move(entry));

        return staff_list.size() - 1;
    }

    inline t_entry_id append_student(student_entry&& entry) {
        student_list.push_back(std::move(entry));

        return student_list.size() - 1;
    }

    bool save() const;
    bool load();
};