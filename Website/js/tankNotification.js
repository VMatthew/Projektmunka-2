$(document).ready(function(){
    $.ajax({
        url : "https://projektmunka2sze.000webhostapp.com/php/data/tankData.php",
        type : "GET",
        success : function(data){
            console.log(data);

            var id = [];
            var tankData = [];

            for(var i in data) {
                id.push(data[i].TIME);
                tankData.push(data[i].tankData);
            }
            $('#notify').append("Jelenlegi állapot : "+id+" : "+ tankData);
           
        
        },
        error : function(data) {

        }
    })});
