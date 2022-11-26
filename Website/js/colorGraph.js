$(document).ready(function(){
    $.ajax({
        url : "https://projektmunka2sze.000webhostapp.com/php/data/color.php",
        type : "GET",
        success : function(data){
            console.log(data);

            var id = [];
            var color = [];

            for(var i in data) {
                id.push(data[i].TIME);
                color.push(data[i].COLLOR);
            }

            var chartdata = {
                labels: id,
                datasets: [
                    {
                        label: "Szín",
                        fill: false,
                        lineTension: 0.1,
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
