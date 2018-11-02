#ifndef EZSERVER_NAMED_CONFIG_H
#define EZSERVER_NAMED_CONFIG_H

#include <boost/di.hpp>
#include <boost/program_options/variables_map.hpp>

namespace ezserver::config::named
{
    /*      Connection Config       */
    auto Port = []{};
    auto Backlog = []{};

    /**
     * Exports this module
     */
    auto config_module = [](const boost::program_options::variables_map& config){
        return boost::di::make_injector(
                boost::di::bind<unsigned short>.named(Backlog).to(config["port"].as<unsigned short>()),
                boost::di::bind<int>.named(Backlog).to(config["backlog"].as<int>())
        );
    };
}

#endif //EZSERVER_NAMED_CONFIG_H
