$(document).ready(function(){
    $.ajax({
        url : "https://projektmunka2sze.000webhostapp.com/php/data/temperature.php",
        type : "GET",
        success : function(data){
            console.log(data);

            var id = [];
            var temperature = [];

            for(var i in data) {
                id.push(data[i].TIME);
                temperature.push(data[i].TEMP);
            }

            var chartdata = {
                labels: id,
                datasets: [
                    {
                        label: "Hőmérséklet",
                        fill: false,
                        lineTension: 0.5,
                        backgroundColor: "rgba(59, 89, 152, 0.75)",
                        borderColor: "rgba(59, 89, 152, 1)",
                        pointHoverBackgroundColor: "rgba(59, 89, 152, 1)",
                        pointHoverBorderColor: "rgba(59, 89, 152, 1)",
                        data: temperature
                    }
                ]
            };

            var ctx = $("#tempChart");
            var LineGraph = new Chart(ctx, {
                type: 'line',
                data: chartdata
            });
        },
        error : function(data) {

        }
    })});
