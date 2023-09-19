const char html_web []  PROGMEM = R"=====(
<!DOCTYPE html> 
<html>
    <head> <!--gõ tên thẻ rồi TAB cho nhanh-->
        <title >ESP32 LED CONTROL</title>  <!--inline-->
        <meta charset="utf-8"> <!--để có thể viết tiếng việt-->
    </head>
    <style>
        h1 {
            color: cadetblue;
            text-align: center;
        }

        .ioButtonOn{
            width: 125px;
            height: 50px;
            background-color: lime;
            margin-left: 30%;
        }
        .ioButtonOff{
            width: 125px;
            height: 50px;
            background-color: rgb(228, 61, 94);
            margin-right: 5%;
        }


        button {
            color: rgb(67, 70, 72);
            
            border-color:aliceblue;
            border-radius: 6px;
            text-align: center;
            
        }

        .outputlabel{
            font-family:Arial, Helvetica, sans-serif;
            color: gray;
            padding-left: 10px;
        }

        .light{
            width: 25px;
            height: 25px;
        }
        .lb{
            font-size: 25px;
            margin-left: 30%;
            margin-bottom:  30px;
        }
        .input_style{
            font-size: 20px;
            width: 90px;
            margin-left: 30%;

        }
        .input_button{
            height: 30px;
            width: 60px;
            margin-bottom: 2%;
        }
    </style>
    <script>
        function LedUpdate()
        {
            var xhttp = new XMLHttpRequest();//thuộc tính của js
            xhttp.onreadystatechange= function ()
            {
                if(this.readyState==4 && this.status==200)
                {
                    var led_state= xhttp.responseText;
                    var datajson = JSON.parse(led_state); //đọc dữ liệu json được esp trả lên web
                    console.log(datajson);
                    if(datajson != null)
                    {   
                        document.getElementById("dataReceived").innerHTML = datajson.led_stt;

                        if(datajson.y0 == "1") document.getElementById("Y0").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-18.png";
                        else if(datajson.y0 == "0") document.getElementById("Y0").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-28.png";
                        
                        if(datajson.y1 == "1") document.getElementById("Y1").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-18.png";
                        else if(datajson.y1 == "0") document.getElementById("Y1").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-28.png";
                        
                        if(datajson.y2 == "1") document.getElementById("Y2").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-18.png";
                        else if(datajson.y2 == "0") document.getElementById("Y2").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-28.png";
                        
                        if(datajson.y3 == "1") document.getElementById("Y3").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-18.png";
                        else if(datajson.y3 == "0") document.getElementById("Y3").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-28.png";
                        
                        if(datajson.y4 == "1") document.getElementById("Y4").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-18.png";
                        else if(datajson.y4 == "0") document.getElementById("Y4").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-28.png";
                        
                        if(datajson.y5 == "1") document.getElementById("Y5").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-18.png";
                        else if(datajson.y5 == "0") document.getElementById("Y5").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-28.png";
                        
                        if(datajson.y6 == "1") document.getElementById("Y6").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-18.png";
                        else if(datajson.y6 == "0") document.getElementById("Y6").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-28.png";
                        
                        if(datajson.y7 == "1") document.getElementById("Y7").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-18.png";
                        else if(datajson.y7 == "0") document.getElementById("Y7").src= "https://www.freeiconspng.com/uploads/bulb-off-icon-28.png";
                        
                        
                    }

                }
            }   
            xhttp.open('GET','/Update',true);//gọi thuộc tính yêu cầu http request với đường dẫn Update cho esp
            xhttp.send(); // gửi request xuống esp
            setTimeout(function(){ LedUpdate() },1000);//1 giây gọi hàm 1laanf và yêu cầu esp trả dữ liệu về web
        }
        
        function OnClicked0()
        {
            var tx = "A0B";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OffClicked0()
        {
            var tx = "C0D";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OnClicked1()
        {
            var tx = "A1B";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OffClicked1()
        {
            var tx = "C1D";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OnClicked2()
        {
            var tx = "A2B";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OffClicked2()
        {
            var tx = "C2D";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OnClicked3()
        {
            var tx = "A3B";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OffClicked3()
        {
            var tx = "C3D";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OnClicked4()
        {
            var tx = "A4B";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OffClicked4()
        {
            var tx = "C4D";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OnClicked5()
        {
            var tx = "A5B";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OffClicked5()
        {
            var tx = "C5D";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OnClicked6()
        {
            var tx = "A6B";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OffClicked6()
        {
            var tx = "C6D";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OnClicked7()
        {
            var tx = "A7B";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }
        function OffClicked7()
        {
            var tx = "C7D";
            var xhttp = new XMLHttpRequest();
            xhttp.open('GET','/Button?Button='+tx,true);
            xhttp.send();
        }

        function submitdata()
        {
            console.log(document.getElementById("input_data").value);
            var tx1 = document.getElementById("input_data").value ;
            if(tx1<0 ||tx1>65535) {
                confirm("Nhập lại");
                window.location.reload();
            }
            else {
                var xhttp = new XMLHttpRequest();
                xhttp.open('GET','/Input?Input='+tx1,true);
                xhttp.send();
            }
        }

    </script>
    <body onload="LedUpdate()">
        
    </body>
    <h1> LED STATE</h1>
    <br>
    <div class="control_data">
        <label class="lb">DATA:</label> 
        <label class="lb" id="dataReceived"  > </label> <br>
        <input type="text" class="input_style" id="input_data">
        <button onclick="submitdata()" class="input_button"> Enter </button>
    </div>

    <div class="screen" >
        <button class="ioButtonOn" id="onY0" onclick="OnClicked0()">ON Y0</button>
        <button class = "ioButtonOff" id="offY0" onclick="OffClicked0()">OFF Y0</button> 
        <label class="outputlabel">Y0:</label>
        <img id="Y0" class="light"  src="https://www.freeiconspng.com/uploads/bulb-off-icon-28.png" >
        <button class="ioButtonOn" id="onY1" onclick="OnClicked1()">ON Y1</button>
        <button class = "ioButtonOff" id="offY1" onclick="OffClicked1()">OFF Y1</button>
        <label class="outputlabel">Y1:</label>
        <img id="Y1" class="light"  src="https://www.freeiconspng.com/uploads/bulb-off-icon-28.png" >
        <button class="ioButtonOn" id="onY2" onclick="OnClicked2()">ON Y2</button>
        <button class = "ioButtonOff" id="offY2" onclick="OffClicked2()">OFF Y2</button>
        <label class="outputlabel">Y2:</label>
        <img id="Y2" class="light"  src="https://www.freeiconspng.com/uploads/bulb-off-icon-28.png" >
        <button class="ioButtonOn" id="onY3" onclick="OnClicked3()">ON Y3</button>
        <button class = "ioButtonOff" id="offY3" onclick="OffClicked3()">OFF Y3</button>
        <label class="outputlabel">Y3:</label>
        <img id="Y3" class="light"  src="https://www.freeiconspng.com/uploads/bulb-off-icon-28.png" >
        <button class="ioButtonOn" id="onY4" onclick="OnClicked4()">ON Y4</button>
        <button class = "ioButtonOff" id="offY4" onclick="OffClicked4()">OFF Y4</button>
        <label class="outputlabel">Y4:</label>
        <img id="Y4" class="light"  src="https://www.freeiconspng.com/uploads/bulb-off-icon-28.png" >
        <button class="ioButtonOn" id="onY5" onclick="OnClicked5()">ON Y5</button>
        <button class = "ioButtonOff" id="offY5" onclick="OffClicked5()">OFF Y5</button>
        <label class="outputlabel">Y5:</label>
        <img id="Y5" class="light"  src="https://www.freeiconspng.com/uploads/bulb-off-icon-28.png" >
        <button class="ioButtonOn" id="onY6" onclick="OnClicked6()">ON Y6</button>
        <button class = "ioButtonOff" id="offY6" onclick="OffClicked6()">OFF Y6</button>
        <label class="outputlabel">Y6:</label>
        <img id="Y6" class="light"  src="https://www.freeiconspng.com/uploads/bulb-off-icon-28.png" >
        <button class="ioButtonOn" id="onY7" onclick="OnClicked7()">ON Y7</button>
        <button class = "ioButtonOff"  id="offY7" onclick="OffClicked7()">OFF Y7</button>
        <label class="outputlabel">Y7:</label>
        <img id="Y7" class="light"  src="https://www.freeiconspng.com/uploads/bulb-off-icon-28.png" >
    </div>


</html>


)=====";
