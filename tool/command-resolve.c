#include "command.h"
#include "interface.h"
#include "object.h"
#include "terminal-colors.h"
#include "util.h"

#include <errno.h>
#include <getopt.h>
#include <string.h>

static long resolve_run(Cli *cli, int argc, char **argv) {
        static const struct option options[] = {
                { "help", no_argument, NULL, 'h' },
                {}
        };
        _cleanup_(freep) char *address = NULL;
        const char *interface = NULL;
        int c;
        long r;

        while ((c = getopt_long(argc, argv, "h", options, NULL)) >= 0) {
                switch (c) {
                        case 'h':
                                printf("Usage: %s resolve INTERFACE\n", program_invocation_short_name);
                                printf("\n");
                                printf("Resolve INTERFACE to the varlink address that implements it.\n");
                                printf("\n");
                                printf("  -h, --help             display this help text and exit\n");
                                return 0;

                        default:
                                return -CLI_ERROR_PANIC;
                }
        }

        interface = argv[optind];
        if (!interface) {
                fprintf(stderr, "Error: INTERFACE expected\n");
                return -CLI_ERROR_MISSING_ARGUMENT;
        }

        r = cli_resolve(cli, interface, &address);
        if (r < 0) {
                fprintf(stderr, "Error resolving interface %s\n", interface);
                return -CLI_ERROR_CANNOT_RESOLVE;
        }

        printf("%s\n", address);

        return 0;
}

static long resolve_complete(Cli *cli, int argc, char **argv, const char *current) {
        if (argc != 1)
                return 0;

        return cli_complete_interfaces(cli, current, false);
}

const CliCommand command_resolve = {
        .name = "resolve",
        .info = "Resolve an interface name to a varlink address",
        .run = resolve_run,
        .complete = resolve_complete
};
