
require('/session.js');

/** The spectrumFeeds() singleton object is returned
 * from the spectrumFeeds() function.
 *
 * opts object:
 *
 *  function callbacks the user may pass in in opts:
 *
 *
 *    newSpectrum(id, description, cFreq,
 *           bandwidth, updatePeriod, bins)
 *
 *    spectrumEnd(id)
 *
 *
 *  methods:
 *
 *    subscribe(id,
 *          spectrumUpdateCallback(id, cFreq, bandwidth,
 *                  updatePeriod, data))
 *
 *
 * 
 */
function spectrumFeeds(io, opts = {}) {

    // We make the returned object be a singleton.
    //
    if(spectrumFeeds.count > 0)
        return spectrumFeeds.obj;
    ++spectrumFeeds.count;

    var obj = {}; // The returned object.

    var spectrums = {}; // List of spectrum feeds


    io.On('newSpectrum', function(id, description, cFreq,
                bandwidth, updatePeriod, bins) {

        spectrums[id] = {
            subscribers: [] // spectrumUpdate user callbacks.
        };

        if(typeof(opts.newSpectrum) === 'function')
            opts.newSpectrum(...arguments);
    });


    io.On('spectrumEnd', function(id) {

        if(spectrums[id] === undefined)
            return;

        if(typeof(opts.spectrumEnd) === 'function')
            // Call the users spectrumEnd callabck
            opts.spectrumEnd(id);

        // Remove all the subscriber callbacks.
        spectrums[id].subscribers = [];

        delete spectrums[id];
    });


    io.On("spectrumUpdate", function(id, cFreq, bandwidth,
            updatePeriod, data) {

        if(spectrums[id] !== undefined &&
                spectrums[id].subscribers.length > 0) {
            args = [...arguments];
            spectrums[id].subscribers.forEach(function(subscriber) {
                // call the users spectrum display callback
                //console.log("subscriber=" + subscriber);
                subscriber([...args]);
            });
        }

    });


    obj.subscribe = function(id, func) {
        if(spectrums[id] !== undefined) {
            io.Emit('spectrumSubscribe', id);
            spectrums[id].subscribers.push(func);
            console.log("added subscriber=" + func);
        }
    };


    obj.unsubscribe = function(id) {
        if(spectrums[id] !== undefined) {
            io.Emit('spectrumUnsubscribe', id);
            // Remove all the subscriber callbacks.
            spectrums[id].subscribers = [];
        }
    };


    spectrumFeeds.obj = obj;
    return obj;
}

spectrumFeeds.count = 0;
spectrumFeeds.obj = null;