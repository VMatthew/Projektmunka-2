$(document).ready(function(){
    $.ajax({
        url : "https://projekt2sze.000webhostapp.com/php/data/color.php",
        type : "GET",
        success : function(data){
            console.log(data);

            var id = [];
            var color = [];

            for(var color in data) {
                id.push(data[color].TIME);
                color.push(data[color].COLLOR);
            }

            var chartdata = {
                labels: id,
                datasets: [
                    {
                        label: "Szín",
                        fill: false,
                        lineTension: 0.5,
                        backgroundColor: "rgba(59, 89, 152, 0.75)",
                        borderColor: "rgba(59, 89, 152, 1)",
                        pointHoverBackgroundColor: "rgba(59, 89, 152, 1)",
                        pointHoverBorderColor: "rgba(59, 89, 152, 1)",
                        data: color
                    }
                ]
            };

            var ctx = $("#colChart");
            var LineGraph = new Chart(ctx, {
                type: 'line',
                data: chartdata
            });
        },
        error : function(data) {

        }
    })});
