//  Copyright (c) 2011 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/runtime/actions/continuation.hpp>
#include <hpx/exception.hpp>
#include <hpx/lcos/local_spinlock.hpp>
#include <hpx/util/unlock_lock.hpp>
#include <hpx/util/stringstream.hpp>
#include <hpx/runtime/threads/thread.hpp>
#include <hpx/runtime/components/component_type.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>
#include <hpx/runtime/components/generic_component_factory.hpp>
#include <hpx/include/performance_counters.hpp>
#include <hpx/performance_counters/counter_creators.hpp>
#include <hpx/lcos/base_lco.hpp>
#include <hpx/components/dataflow/server/dataflow.hpp>
#include <hpx/components/dataflow/server/dataflow_trigger.hpp>
#include <hpx/include/iostreams.hpp>

#include <hpx/util/portable_binary_iarchive.hpp>
#include <hpx/util/portable_binary_oarchive.hpp>

#include <boost/serialization/version.hpp>

HPX_REGISTER_COMPONENT_MODULE();

typedef hpx::components::managed_component<hpx::lcos::server::dataflow> dataflow_type;

HPX_REGISTER_MINIMAL_GENERIC_COMPONENT_FACTORY_EX(
    dataflow_type, bright_future_dataflow, true);

HPX_DEFINE_GET_COMPONENT_TYPE(dataflow_type::wrapped_type);

HPX_REGISTER_ACTION_EX(
    hpx::lcos::server::dataflow::connect_action
  , dataflow_type_connect_action)

typedef hpx::components::managed_component<hpx::lcos::server::dataflow_trigger>
    dataflow_trigger_type;

HPX_REGISTER_MINIMAL_GENERIC_COMPONENT_FACTORY_EX(
    dataflow_trigger_type,
    bright_future_dataflow_trigger, true);

HPX_DEFINE_GET_COMPONENT_TYPE(dataflow_trigger_type::wrapped_type);

HPX_REGISTER_ACTION_EX(
    hpx::lcos::server::dataflow_trigger::connect_action
  , dataflow_trigger_type_connect_action)

// HPX_REGISTER_ACTION_EX(
//     hpx::lcos::server::dataflow_trigger::init_action
//   , dataflow_trigger_type_init_action
// )

///////////////////////////////////////////////////////////////////////////////
namespace hpx { namespace lcos { namespace server { namespace detail
{
    // the counter data instance
    dataflow_counter_data dataflow_counter_data_;

    boost::int64_t get_constructed_count()
    {
        lcos::local_spinlock::scoped_lock l(dataflow_counter_data_.mtx_);
        return dataflow_counter_data_.constructed_;
    }

    boost::int64_t get_initialized_count()
    {
        lcos::local_spinlock::scoped_lock l(dataflow_counter_data_.mtx_);
        return dataflow_counter_data_.initialized_;
    }

    boost::int64_t get_fired_count()
    {
        lcos::local_spinlock::scoped_lock l(dataflow_counter_data_.mtx_);
        return dataflow_counter_data_.fired_;
    }

    // call this to register all counter types for dataflow entries
    void register_counter_types()
    {
        performance_counters::generic_counter_type_data const counter_types[] =
        {
            { "/lcos/dataflow/constructed", performance_counters::counter_raw,
              "returns the number of constructed dataflow objects",
              HPX_PERFORMANCE_COUNTER_V1,
              boost::bind(&performance_counters::locality_raw_counter_creator,
                  _1, get_constructed_count, _2),
              &performance_counters::locality_counter_discoverer
            },
            { "/lcos/dataflow/initialized", performance_counters::counter_raw,
              "returns the number of initialized dataflow objects",
              HPX_PERFORMANCE_COUNTER_V1,
              boost::bind(&performance_counters::locality_raw_counter_creator,
                  _1, get_initialized_count, _2),
              &performance_counters::locality_counter_discoverer
            },
            { "/lcos/dataflow/fired", performance_counters::counter_raw,
              "returns the number of fired dataflow objects",
              HPX_PERFORMANCE_COUNTER_V1,
              boost::bind(&performance_counters::locality_raw_counter_creator,
                  _1, get_fired_count, _2),
              &performance_counters::locality_counter_discoverer
            }
        };
        performance_counters::install_counter_types(
            counter_types, sizeof(counter_types)/sizeof(counter_types[0]));
    }
}}}}

