#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <dns_sd.h>
#include <ev++.h>

#include "build_version.h"


inline void check_dnsservice_errors(DNSServiceErrorType& e, const std::string& func_name) {
    std::string error(func_name);
    error += ": ";

    switch (e) {
        case kDNSServiceErr_NoError:
            return;
        case kDNSServiceErr_NameConflict:
            error += "Name in use, please choose another";
            break;
        case kDNSServiceErr_Invalid:
            error += "An invalid index or character was passed";
            break;
        case kDNSServiceErr_BadReference:
            error += "Bad reference";
            break;
        case kDNSServiceErr_BadParam:
            error += "Bad parameter value passed to function.";
            break;
        default:
            error += "Unknown error code... ";
            error += std::to_string(e);
    }

    throw std::runtime_error(error.c_str());
}

int main(int argc, char *argv[]) {
    if (2 != argc) {
        std::cerr << argv[0] << " <announce file>" << std::endl;
        std::cerr << "Version: " << VERSION << std::endl;
        return -1;
    }

    std::vector<std::string> _domains;

    DNSServiceRef serviceRef = 0;
    int dns_sd_fd = -1;

    try {
        std::ifstream f(argv[1], std::ios::in | std::ios::binary);

        if (!f.is_open()) {
            std::cerr << "Unable to open file: " << argv[1] << std::endl;
            return -2;
        }

        for (std::string line; std::getline(f, line); /**/) {
            if (line.length() != 0) _domains.push_back(line);
        }

        DNSServiceErrorType error = kDNSServiceErr_NoError;

        // Setup the mDNS context...
        error = DNSServiceCreateConnection(&serviceRef);
        check_dnsservice_errors(error, "DNSServiceCreateConnection");

        for (auto &d : _domains) {
            std::cout << "Registering Domain: " << d << std::endl;

        }
    } catch (std::exception const& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }

    DNSServiceRefDeallocate(serviceRef);

    return 0;
}
