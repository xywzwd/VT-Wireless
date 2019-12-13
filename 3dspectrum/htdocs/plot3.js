function getData() {

    var cnt = 0;

    setInterval(function(){
        var text = document.getElementById("data").innerText;
        // console.log("-------------" + text);
        var arr = text.split(",");
        var power = arr.splice(5);
        cnt++;
        // console.log("--------" + cnt);
        showPlot(power, cnt);
    }, 500);


}

function showPlot(power, cnt) {
    //plot1
    if (cnt < 2) {
        var layout =  {
                autosize: true,
                margin: {
                    autoexpand: true,
                },

                title: {
                    text:'Waterfall',
                    font: {
                        family: 'Monospace',
                        size: 24
                    },
                    xref: 'paper',
                    x: 0.05,
                },
                xaxis: {
                    title: {
                        text: 'Power',
                        font: {
                            family: 'monospace',
                            size: 18,
                            color: '#7f7f7f'
                        }
                    },
                },
                yaxis: {
                    title: {
                        text: 'Time',
                        font: {
                            family: 'monospace',
                            size: 18,
                            color: '#7f7f7f'
                        }
                    }
                }
            };
            
        Plotly.plot('plot1',[{
            x: power,
            type:'line',
            
        }], layout);
    }
    
    // var layout =  {
    //             autosize: true,
    //             margin: {
    //                 autoexpand: true,
    //             }
    //         };
            
    //     Plotly.plot('plot1',[{
    //         x: power,
    //         type:'line',
    //         layout
    //     }]);

        
        // setInterval(function(){
            Plotly.extendTraces('plot1',{x: [power]}, [0]);
            if(cnt > 0) 
            {
                Plotly.relayout('plot1', {
                    yaxis: {range: [cnt, cnt + 50],
                            title: {
                            text: 'Time',
                            font: {
                                family: 'monospace',
                                size: 18,
                                color: '#7f7f7f'
                            }
                        }
                    },
                    xaxis: {range: [0, 0.001],
                            title: {
                            text: 'Power',
                            font: {
                                family: 'monospace',
                                size: 18,
                                color: '#7f7f7f'
                            }
                        },
                    }
                })
            }


//plot2
        if (cnt < 2) {
            var layout2 =  {
                autosize: true,
                margin: {
                    autoexpand: true,
                },

                title: {
                    text:'Throughput',
                    font: {
                        family: 'Monospace',
                        size: 24
                    },
                    xref: 'paper',
                    x: 0.05,
                },
                xaxis: {
                    title: {
                        text: 'Time (ms)',
                        font: {
                            family: 'monospace',
                            size: 18,
                            color: '#7f7f7f'
                        }
                    },
                },
                yaxis: {
                    title: {
                        text: 'Throughput',
                        font: {
                            family: 'monospace',
                            size: 18,
                            color: '#7f7f7f'
                        }
                    }
                }
            };
            Plotly.plot('plot2',[{
                   y:[power/0.005],
                   type:'bar',
                   autoscale:false
                   }], layout2);
        }
    
            
                
                
        Plotly.extendTraces('plot2',{ y:[power]}, [0]);
        if(cnt > 0) {
            Plotly.relayout('plot2',{
                xaxis: {
                    range: [cnt, cnt + 300],
                     title: {
                            text: 'Time (ms)',
                            font: {
                                family: 'monospace',
                                size: 18,
                                color: '#7f7f7f'
                            }
                        },
                },
                 yaxis: {
                    title: {
                        text: 'Throughput',
                        font: {
                            family: 'monospace',
                            size: 18,
                            color: '#7f7f7f'
                        }
                    }
                }

            });
        }
    //plot3
        var layout3 = {
        autosize: true,
        margin: {
            autoexpand: true
        },
        title: {
            text:'Spectrum',
            font: {
                family: 'Monospace',
                size: 24
            },
            xref: 'paper',
            x: 0.05,
        },
        xaxis: {
            tickmode: 'array',
            tickvals: [0, power.length / 2, power.length -1],
            ticktext: ["914.00", "914.50", "915.00"],
            title: {
                text: 'Frequency Bandwith (MHz)',
                font: {
                    family: 'monospace',
                    size: 18,
                    color: '#7f7f7f'
                }
            },
        },
        yaxis: {
            title: {
                text: 'Power',
                font: {
                    family: 'monospace',
                    size: 18,
                    color: '#7f7f7f'
                }
            }
        }
    };

        Plotly.newPlot('plot3', [{
            y: power,
            type: 'line',
            // layout3
        }], layout3);
}