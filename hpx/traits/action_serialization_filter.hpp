//  Copyright (c) 2007-2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_TRAITS_ACTION_SERIALIZATION_FILTER_FEB_14_2013_0903PM)
#define HPX_TRAITS_ACTION_SERIALIZATION_FILTER_FEB_14_2013_0903PM

#include <hpx/hpx_fwd.hpp>
#include <hpx/util/binary_filter.hpp>

namespace hpx { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    // Customization point for action stack size
    template <typename Action, typename Enable>
    struct action_serialization_filter
    {
        static util::binary_filter* call() 
        {
            return 0;   // by default actions don't have a serialization filter
        }
    };
}}

#endif

