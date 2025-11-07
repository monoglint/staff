#include <iostream>
#include <limits>

#include "api.hh"

template <typename T>
static inline T in_t() {
    T in;
    std::cin >> in;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return in;
}

static inline std::string in_string() {
    std::string str;
    std::getline(std::cin, str);

    return str;
}

static inline std::string prompt(const std::string& text) {
    std::cout << text << ": ";
    return in_string();
}

template <typename T>
static inline T prompt(const std::string& text) {
    std::cout << text << ": ";
    return in_t<T>();
}

static void add_staff(staff_session& active_session) {
    system("cls");
    std::string name = prompt("Please provide a name");
    int days = prompt<int>("How many periods max is the staff willing to be on duty for?");

    while (days > 6) {
        days = prompt<int>("Please enter a value between 1 and 6.");
    }

    std::unordered_set<e_period_index> available_periods;

    for (t_period_index_base period = 0; period < (t_period_index_base)e_period_index::_MAX; period++) {
        system("cls");

        std::cout << "Enter period availability:\n\n";

        switch ((t_period_index_base)period) {
            case 0: std::cout << "A day\n\n__A__ lunch: "; break;
            case 1: std::cout << "A day\n\n__B__ lunch: "; break;
            case 2: std::cout << "A day\n\n__C__ lunch: "; break;
            case 3: std::cout << "B day\n\n__A__ lunch: "; break;
            case 4: std::cout << "B day\n\n__B__ lunch: "; break;
            case 5: std::cout << "B day\n\n__C__ lunch: "; break;
        }

        char in = prompt<char>("Does the teacher want to watch this period? (y/n)");

        while (in != 'y' && in != 'n') {
            in = prompt<char>("Please enter 'y' or 'n'");
        }

        if (in == 'y')
            available_periods.insert((e_period_index)period);
    }
    
    active_session.add_staff(name, days, available_periods);

    std::cout << "Added staff.\nPress enter to continue.\n";
    std::cin.get();
}

static void add_student(staff_session& active_session) {
    system("cls");
    std::string name = prompt("Please provide a name");


}

static void remove(staff_session& active_session) {
    system("cls");
    std::string name = prompt("Please provide a name to remove");
}

static void make_schedule(staff_session& active_session) {
    system("cls");
    std::cout << active_session.print_distribution_export().str();

    std::cout << "Press enter to continue.\n";
    std::cin.get();
}

int main() {
    staff_session active_session("staffsave.ls");

    while (true) {
        system("cls");

        std::cout << active_session.print_state().str();

        int input = prompt<int>("(1) Add Staff\n(2) Add Student\n(3) Remove\n(4) Make Schedule\n(5) Reload\n(Etc) Quit\n");
        
        switch (input) {
            case 1:
                add_staff(active_session);
                break;
            case 2:
                add_student(active_session);
                break;
            case 3:
                remove(active_session);
                break;
            case 4:
                make_schedule(active_session);
                break;
            case 5:
                break;
            default:
                goto exit;
        }
    }

    exit:

    return 0;
}