const char html []  PROGMEM = R"=====(
<!DOCTYPE html> 
<html>
    <head> <!--gõ tên thẻ rồi TAB cho nhanh-->
        <title >ESP32 CONFIG TEST</title>  <!--inline-->
        <meta charset="utf-8"> <!--để có thể viết tiếng việt-->
    </head>
    <style>

    </style>
    <script>
        function config()
        {
            console.log("OFF CLICK");
            var tx1 = document.getElementById("id").value + "-" + document.getElementById("pass").value;
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Input?Input='+tx1,true);
            xhttp.send();
            if(confirm('Đã gửi thành công.')){
            window.location.reload();  
            }

        }
    </script>
    <body >
        
    </body>


    <input type="text" id="id" ><br>
    <input type="password" id="pass"><br>
    <button onclick="config()">ENTER</button>


</html>

)=====";
