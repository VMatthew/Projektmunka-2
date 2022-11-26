$(document).ready(function(){
    $.ajax({
        url : "https://projektmunka2sze.000webhostapp.com/php/data/moisture.php",
        type : "GET",
        success : function(data){
            console.log(data);

            var id = [];
            var soilMoisture = [];

            for(var i in data) {
                id.push(data[i].TIME);
                soilMoisture.push(data[i].MOIST);
            }

            var chartdata = {
                labels: id,
                datasets: [
                    {
                        label: "Nedvesség",
                        fill: false,
                        lineTension: 0.5,
                        backgroundColor: "rgba(59, 89, 152, 0.75)",
                        borderColor: "rgba(59, 89, 152, 1)",
                        pointHoverBackgroundColor: "rgba(59, 89, 152, 1)",
                        pointHoverBorderColor: "rgba(59, 89, 152, 1)",
                        data: soilMoisture
                    }
                ]
            };

            var ctx = $("#moistChart");
            var LineGraph = new Chart(ctx, {
                type: 'line',
                data: chartdata
            });
        },
        error : function(data) {

        }
    })});
