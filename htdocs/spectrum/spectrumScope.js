<!DOCTYPE html>
<html lang=en>
<head>
    <meta charset="utf-8"/>
    <script src='/load.js'></script>

    <script>

// Note: In order to keep webDesktop and scope usable by other software
// projects we cannot use require() in them so we must put a longer list
// of dependences in this CRTS specific code.

require('/spectrum/spectrum.js');

onload = function() {

    function SpectrumDisplay(desc, onclose_in = null) {

        var ampMax, ampMin;
        var cFreq = 0, bandwidth = 0, bins = 0;

        var onclose = onclose_in;

        var scope = new Scope;

        var app = new WDApp('Spectrum ' + desc,
                scope.getElement(),
                function() {
                    console.log('Removing spectrum scope: ' + desc);
                    if(onclose) onclose();
                app = null;
        });

        this.destroy = function() {
            if(app) app.close();
            app = null;
        };
        this.draw = function(x, y, _cFreq, _bandwidth, _updatePeriod) {

            // We have new spectrum data.
            let l = x.length;

            if(cFreq !== _cFreq || bandwidth !== _bandwidth || l !== bins) {

                // The parameters have changed since the last draw call.
                cFreq = _cFreq;
                bandwidth = _bandwidth;
                bins = l;

                // Delete the old ampM* arrays.
                ampMax = [];
                ampMin = [];

                for(let i=0; i<l; ++i) {
                    // Initialize the arrays.
                    ampMax[i] = Number.MIN_VALUE;
                    ampMin[i] = Number.MAX_VALUE;
                }
            }

            for(let i=0; i<l; ++i) {
                if(y[i] > ampMax[i])
                    ampMax[i] = y[i];
                if(y[i] < ampMin[i])
                    ampMin[i] = y[i];
            }

            scope

            scope.draw(x, y, 0);
            scope.draw(x, ampMax, 1);
            scope.draw(x, ampMin, 2);
            scope.draw();
        };
    }

    createSession(function(io) {

        //createSpectrumFeeds(io);

        createSpectrumFeeds(io, SpectrumDisplay);
    });
};
    </script>


    <title>CRTS</title>
</head>
<body>

    <h1>Spectrum Feed Test</h1>

    <p>This test launches a spectrum display for each new spectrum
    feed that it sees from the webSocket connection to the server.</p>

    <p><a href=".">Back to index</a></p>

</body>
</html>
