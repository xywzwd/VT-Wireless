#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <uhd/usrp/multi_usrp.hpp>

#include "crts/debug.h"
#include "crts/Filter.hpp"
#include "crts/Control.hpp"
#include "crts/crts.hpp"


class Rx;


class RxControl: public CRTSControl
{
    public:

        // This is a very course/crude control that lets the controllers
        // just have at the guts of this RX device.  This may not be what
        // you want, but this is very simple way to give control to the
        // controller.  It's an obvious first try, example, for RX filter
        // control/controller connectivity.

        uhd::usrp::multi_usrp::sptr &usrp;
        uhd::rx_streamer::sptr &rx_stream;

    private:

        RxControl(CRTSFilter *rxFilter, std::string controlName,
                uhd::usrp::multi_usrp::sptr &usrp_in,
                uhd::rx_streamer::sptr &rx_stream_in):
            CRTSControl(rxFilter, controlName),
            usrp(usrp_in), rx_stream(rx_stream_in)
        {
            DSPEW();
        };

        ~RxControl(void) { DSPEW(); };


    // The Rx is the only object that can make an RxControl given the
    // private constructor that only Rx can access.
    friend Rx;
};


#define DEFAULT_RXCONTROL_NAME "rx"
