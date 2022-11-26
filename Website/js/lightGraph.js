$(document).ready(function(){
    $.ajax({
        url : "https://projekt2sze.000webhostapp.com/php/data/light.php",
        type : "GET",
        success : function(data){
            console.log(data);

            var id = [];
            var light = [];

            for(var light in data) {
                id.push(data[light].TIME);
                light.push(data[light].LIGHT);
            }

            var chartdata = {
                labels: id,
                datasets: [
                    {
                        label: "Fényerő",
                        fill: false,
                        lineTension: 0.5,
                        backgroundColor: "rgba(59, 89, 152, 0.75)",
                        borderColor: "rgba(59, 89, 152, 1)",
                        pointHoverBackgroundColor: "rgba(59, 89, 152, 1)",
                        pointHoverBorderColor: "rgba(59, 89, 152, 1)",
                        data: light
                    }
                ]
            };

            var ctx = $("#lightChart");
            var LineGraph = new Chart(ctx, {
                type: 'line',
                data: chartdata
            });
        },
        error : function(data) {

        }
    })});
