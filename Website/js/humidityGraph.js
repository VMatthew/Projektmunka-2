$(document).ready(function(){
    $.ajax({
        url : "https://projekt2sze.000webhostapp.com/php/data/humidity.php",
        type : "GET",
        success : function(data){
            console.log(data);

            var id = [];
            var humidity = [];

            for(var hum in data) {
                id.push(data[temp].TIME);
                humidity.push(data[hum].HUM);
            }

            var chartdata = {
                labels: id,
                datasets: [
                    {
                        label: "Páratartalom",
                        fill: false,
                        lineTension: 0.5,
                        backgroundColor: "rgba(59, 89, 152, 0.75)",
                        borderColor: "rgba(59, 89, 152, 1)",
                        pointHoverBackgroundColor: "rgba(59, 89, 152, 1)",
                        pointHoverBorderColor: "rgba(59, 89, 152, 1)",
                        data: humidity
                    }
                ]
            };

            var ctx = $("#humChart");
            var LineGraph = new Chart(ctx, {
                type: 'line',
                data: chartdata
            });
        },
        error : function(data) {

        }
    })});
