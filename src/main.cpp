#include "types/password.hpp"
#include "types/options.hpp"
#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <unistd.h>

using std::cin;
using std::stoi;

const char *usage =
    "Usage: pw [OPTION]... SERVICE USERNAME\n"
    "Generate deterministic passwords using a master password.\n\n"
    "  -l, --length LENGTH      Length of the resulting password.\n"
    "  -c, --changes CHANGES    How many times the password has been changed.\n"
    "  -a, --alpha              Generate an alphanumeric password.\n"
    "  -m, --master PASS        Use PASS as the master password.\n"
    "      --config FILE        Path to a config file.\n"
    "\n";

Options
parseopts(int argc, char *const *argv) {
    Options options;

    while(true) {
        /* Define a list of long options that the program will parse and
         * accept, this is specifically for getopt_long. */
        static option long_options[] = {
            {"length",    required_argument,  0,  'l'},
            {"changes",   required_argument,  0,  'c'},
            {"alpha",     no_argument,        0,  'a'},
            {"master",    required_argument,  0,  'm'},
            {"config",    required_argument,  0,   0 },
            {"database",  required_argument,  0,   0 },
            {"help",      no_argument,        0,  'h'},
            {0,           0,                  0,   0 }
        };

        /* Parse an argument. */
        int option_index = 0;
        int option_value = getopt_long(argc, argv, "ham:c:l:", long_options, &option_index);

        switch(option_value) {
            case 'c':
                options.changes = atoi(optarg);
                break;

            case 'l':
                options.length = atoi(optarg);
                break;

            case 'a':
                options.alphanumeric = true;
                break;

            case 'm':
                options.master = optarg;
                break;

            case 'h':
                options.help = true;
                break;

            case -1:
                /* Any arguments left over that are not flag arguments are
                 * pushed to the end, store the address left by getopt. */
                if(optind < argc) {
                    options.argc = argc - optind;
                    options.argv = &argv[optind];
                }

                return options;

            case '?':
            default:
                break;
        }
    }
}

int
main(int argc, char *const *argv) {
    /* Parse command line options. */
    Options opt = parseopts(argc, argv);
    if(opt.help) {
        __builtin_printf(usage);
        return 0;
    }

    if(opt.argc == 0) {
        return 0;
    }

    /* Command line arguments are used for generating passwords in place. If no
     * arguments are specified, the password interface launches instead. */
    if(argc > 1) {
        /* Fail if no master password has been found through any means. */
        if(!opt.master) {
            __builtin_printf("No master password specified. Can't generate a password.\n\n");
            return 1;
        }

        /* Generate a password on the spot for the arguments passed. */
        Password password {
            opt.argv[0],
            opt.argv[1],
            opt.length,
            opt.changes
        };

        /* Switch to alphanumeric if specified, these are passwords without any
         * special characters (&!.? etc) - some sites require passwords without
         * special characters, god knows why, shitty practices. */
        if(opt.alphanumeric) {
            password.set_encoder(hex_encode);
        }

        __builtin_printf("%s\n", password.generate(opt.master).c_str());
    }

    return 0;
}
