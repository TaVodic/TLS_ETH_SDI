void website(EthernetClient client, Switcher &sw1, Switcher &sw2, Switcher &sw3, const char* status1, const char* status2, const char* save, const char* version){
     client.print("<!DOCTYPE html>");
     client.print(F(""));
     client.print(F("<title>TLS configuration</title>"));
     client.print(F("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js\"></script>"));
     client.print(F("<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js\"></script>"));
     client.print(F("<html>"));
     client.print(F("<head>"));
     client.print(F("<link rel=\"icon\" href=\"data:,\">"));
     client.print(F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"));
     client.print(F("<style>"));
     client.print(F("#cover {"));
     client.print(F("position: fixed;"));
     client.print(F("height: 100%;"));
     client.print(F("width: 100%;"));
     client.print(F("top: 0;"));
     client.print(F("left: 0;"));
     client.print(F("background: rgb(255, 255, 255);"));
     client.print(F("z-index: 9999;"));
     client.print(F("}"));
     client.print(F("body {"));
     client.print(F("font-family: 'Trebuchet MS', 'Lucida Sans Unicode', 'Lucida Grande', 'Lucida Sans', Arial, sans-serif, serif;"));
     client.print(F("}"));
     client.print(F(".loader {"));
     client.print(F("position: absolute;"));
     client.print(F("left: 50%;"));
     client.print(F("top: 300px;"));
     client.print(F("margin-left: -200px;"));
     client.print(F("width: 400px;"));
     client.print(F("height: 350px;"));
     client.print(F("background-color: #f2f2f2;"));
     client.print(F("border-radius: 20px;"));
     client.print(F("border: 1px solid rgb(71, 71, 71);"));
     client.print(F("}"));
     client.print(F("#loader {"));
     client.print(F("position: absolute;"));
     client.print(F("margin: 30px 55px;"));
     client.print(F("border: 20px solid #dddddd;"));
     client.print(F("border-radius: 50%;"));
     client.print(F("border-top: 20px solid rgb(71, 71, 71);"));
     client.print(F("width: 250px;"));
     client.print(F("height: 250px;"));
     client.print(F("-webkit-animation: spin 1s linear infinite;"));
     client.print(F("animation: spin 1s linear infinite;"));
     client.print(F("}"));
     client.print(F("@-webkit-keyframes spin {"));
     client.print(F("0% {"));
     client.print(F("-webkit-transform: rotate(0deg);"));
     client.print(F("}"));
     client.print(F("100% {"));
     client.print(F("-webkit-transform: rotate(360deg);"));
     client.print(F("}"));
     client.print(F("}"));
     client.print(F("@keyframes spin {"));
     client.print(F("0% {"));
     client.print(F("transform: rotate(0deg);"));
     client.print(F("}"));
     client.print(F("100% {"));
     client.print(F("transform: rotate(360deg);"));
     client.print(F("}"));
     client.print(F("}"));
     client.print(F("div.loader p {"));
     client.print(F("position: absolute;"));
     client.print(F("width: 100%;"));
     client.print(F("text-align: center;"));
     client.print(F("top: 50%;"));
     client.print(F("margin-top: -9px;"));
     client.print(F("font-size: 25px;"));
     client.print(F("}"));
     client.print(F("/* BODY */"));
     client.print(F("input[type=text],"));
     client.print(F("input[type=number],"));
     client.print(F("select,"));
     client.print(F("textarea {"));
     client.print(F("width: 100%;"));
     client.print(F("padding: 12px;"));
     client.print(F("border: 1px solid #ccc;"));
     client.print(F("border-radius: 4px;"));
     client.print(F("box-sizing: border-box;"));
     client.print(F("margin-top: 6px;"));
     client.print(F("margin-bottom: 16px;"));
     client.print(F("resize: vertical;"));
     client.print(F("}"));
     client.print(F("div#buttons {"));
     client.print(F("clear: both;"));
     client.print(F("padding: 0px 0 30px 0;"));
     client.print(F("text-align: center;"));
     client.print(F("}"));
     client.print(F("div#buttons>* {"));
     client.print(F("display: inline-block;"));
     client.print(F("}"));
     client.print(F(".button {"));
     client.print(F("color: white;"));
     client.print(F("padding: 12px 20px;"));
     client.print(F("border: none;"));
     client.print(F("border-radius: 4px;"));
     client.print(F("cursor: pointer;"));
     client.print(F("width: 100px;"));
     client.print(F("margin: 0 4px;"));
     client.print(F("text-decoration: none;"));
     client.print(F("font-size: 14px;"));
     client.print(F("font-family: 'Trebuchet MS', 'Lucida Sans Unicode', 'Lucida Grande', 'Lucida Sans', Arial, sans-serif, serif;"));
     client.print(F("}"));
     client.print(F(".button#refresh {"));
     client.print(F("background-color: #666;"));
     client.print(F("}"));
     client.print(F(".button#refresh:hover {"));
     client.print(F("background-color: rgb(134, 134, 134);"));
     client.print(F("}"));
     client.print(F("input[type=submit].button {"));
     client.print(F("width: 120px;"));
     client.print(F("font-size: 14px;"));
     client.print(F("background-color: #4CAF50;"));
     client.print(F("}"));
     client.print(F("input[type=submit].button:hover {"));
     client.print(F("background-color: #3e8f42;"));
     client.print(F("}"));
     client.print(F(".button#save {"));
     client.print(F("background-color: rgb(255, 65, 65);"));
     client.print(F("margin-left: 3px;"));
     client.print(F("}"));
     client.print(F(".button#save:hover {"));
     client.print(F("background-color: rgb(207, 0, 0);"));
     client.print(F("}"));
     client.print(F("/* Container */"));
     client.print(F(".container {"));
     client.print(F("border-radius: 5px;"));
     client.print(F("background-color: #f2f2f2;"));
     client.print(F("padding: 20px;"));
     client.print(F("padding-top: 0;"));
     client.print(F("width: 60%;"));
     client.print(F("margin: 5%;"));
     client.print(F("box-sizing: border-box;"));
     client.print(F("float: left;"));
     client.print(F("}"));
     client.print(F(".container#a {"));
     client.print(F("margin: 0 20% 20px 20%;"));
     client.print(F("}"));
     client.print(F(".switchers#ATEM {"));
     client.print(F("width: 100%;"));
     client.print(F("display: inline-block;"));
     client.print(F("}"));
     client.print(F(".switchers {"));
     client.print(F("width: 49%;"));
     client.print(F("display: inline-block;"));
     client.print(F("margin-top: 0;"));
     client.print(F("}"));
     client.print(F(".switchers label {"));
     client.print(F("display: block;"));
     client.print(F("text-align: center;"));
     client.print(F("}"));
     client.print(F(".switchers input[type=text] {"));
     client.print(F("width: 70%;"));
     client.print(F("padding: 12px;"));
     client.print(F("border: 1px solid #ccc;"));
     client.print(F("border-radius: 4px;"));
     client.print(F("box-sizing: border-box;"));
     client.print(F("margin: 10px 15% 10px 15%;"));
     client.print(F("resize: vertical;"));
     client.print(F("text-align: center;"));
     client.print(F("}"));
     client.print(F("#channels #title {"));
     client.print(F("font-size: large;"));
     client.print(F("height: 50px;"));
     client.print(F("}"));
     client.print(F("#channels input[type=number] {"));
     client.print(F("padding: 12px;"));
     client.print(F("border: 1px solid #ccc;"));
     client.print(F("border-radius: 4px;"));
     client.print(F("box-sizing: border-box;"));
     client.print(F("margin-top: 6px;"));
     client.print(F("margin-bottom: 6px;"));
     client.print(F("resize: vertical;"));
     client.print(F("}"));
     client.print(F(".container#a table {"));
     client.print(F("margin: 0 10% 10px 10%;"));
     client.print(F("width: 80%;"));
     client.print(F("vertical-align: top;"));
     client.print(F("}"));
     client.print(F(".container#a table th#firstCol {"));
     client.print(F("width: 100px;"));
     client.print(F("}"));
     client.print(F(".container#a table td {"));
     client.print(F("padding: 0px 10px;"));
     client.print(F("}"));
     client.print(F("/**********/"));
     client.print(F("#connected {"));
     client.print(F("color: #4CAF50;"));
     client.print(F("margin-top: 0px;"));
     client.print(F("text-align: center;"));
     client.print(F("}"));
     client.print(F("#disconnected {"));
     client.print(F("color: red;"));
     client.print(F("margin-top: 0px;"));
     client.print(F("text-align: center;"));
     client.print(F("}"));
     client.print(F(".headline {"));
     client.print(F("text-align: center;"));
     client.print(F("}"));
     client.print(F("/* ENABLE switches */"));
     client.print(F(".container#a table#switches {"));
     client.print(F("width: 70.5%;"));
     client.print(F("margin: 0 15% 0px 14%;"));
     client.print(F("}"));
     client.print(F(".switch {"));
     client.print(F("display: inline-block;"));
     client.print(F("position: relative;"));
     client.print(F("margin-bottom: 10px;"));
     client.print(F("left: 50%;"));
     client.print(F("transform: translateX(-24px);"));
     client.print(F("}"));
     client.print(F(".switch-input {"));
     client.print(F("display: none;"));
     client.print(F("}"));
     client.print(F(".switch-label {"));
     client.print(F("display: block;"));
     client.print(F("width: 48px;"));
     client.print(F("height: 24px;"));
     client.print(F("text-indent: 150%;"));
     client.print(F("clip: rect(0 0 0 0);"));
     client.print(F("color: transparent;"));
     client.print(F("user-select: none;"));
     client.print(F("}"));
     client.print(F(".switch-label::before,"));
     client.print(F(".switch-label::after {"));
     client.print(F("content: \"\";"));
     client.print(F("display: block;"));
     client.print(F("position: absolute;"));
     client.print(F("cursor: pointer;"));
     client.print(F("}"));
     client.print(F(".switch-label::before {"));
     client.print(F("width: 100%;"));
     client.print(F("height: 100%;"));
     client.print(F("background-color: rgba(255, 0, 0, 0.699);"));
     client.print(F("border-radius: 9999em;"));
     client.print(F("-webkit-transition: background-color 0.25s ease;"));
     client.print(F("transition: background-color 0.25s ease;"));
     client.print(F("}"));
     client.print(F(".switch-label::after {"));
     client.print(F("top: 0;"));
     client.print(F("left: 0;"));
     client.print(F("width: 24px;"));
     client.print(F("height: 24px;"));
     client.print(F("border-radius: 50%;"));
     client.print(F("background-color: #fff;"));
     client.print(F("box-shadow: 0 0 2px rgba(0, 0, 0, 0.45);"));
     client.print(F("-webkit-transition: left 0.25s ease;"));
     client.print(F("transition: left 0.25s ease;"));
     client.print(F("}"));
     client.print(F(".switch-input:checked+.switch-label::before {"));
     client.print(F("background-color: #89c12d;"));
     client.print(F("}"));
     client.print(F(".switch-input:checked+.switch-label::after {"));
     client.print(F("left: 24px;"));
     client.print(F("}"));
     client.print(F("@media only screen and (min-width: 1600px) {"));
     client.print(F(".container#a {"));
     client.print(F("width: 46%;"));
     client.print(F("margin: 0 27% 20px 27%;"));
     client.print(F("}"));
     client.print(F("}"));
     client.print(F("@media only screen and (max-width: 1810px) {"));
     client.print(F(".container#c label {"));
     client.print(F("margin: 0;"));
     client.print(F("}"));
     client.print(F("}"));
     client.print(F("#footer {"));
     client.print(F("position: fixed;"));
     client.print(F("bottom: 10px;"));
     client.print(F("right: 10px;"));
     client.print(F("}"));
     client.print(F("/* BMD input list */"));
     client.print(F(".row {"));
     client.print(F("text-align: center;"));
     client.print(F("width: 50%;"));
     client.print(F("margin: 0px auto 50px auto;"));
     client.print(F("}"));
     client.print(F(".column {"));
     client.print(F("box-sizing: border-box;"));
     client.print(F("float: left;"));
     client.print(F("width: 50%;"));
     client.print(F("padding: 10px;"));
     client.print(F("}"));
     client.print(F(".row:after {"));
     client.print(F("box-sizing: border-box;"));
     client.print(F("content: \"\";"));
     client.print(F("display: table;"));
     client.print(F("clear: both;"));
     client.print(F("}"));
     client.print(F("</style>"));
     client.print(F("</head>"));
     client.print(F("<body>"));
     client.print(F("<script>"));
     client.print(F("$(document).ready(function () {"));
     client.print(F("$(\"#cover\").hide();"));
     client.print(F("$(\".loader\").hide();"));
     client.print(F("if (document.URL.indexOf(\"IPaddr\") >= 0) {"));
     client.print(F("$(\".loader\").show();"));
     client.print(F("$(\"#cover\").show();"));
     client.print(F("setTimeout(function () {"));
     client.print(F("window.location.replace(\"/\");"));
     client.print(F("}, 1);"));
     client.print(F("}"));
     client.print(F("const form = document.getElementById(\"theForm\");"));
     client.print(F("form.addEventListener('submit', (event) => {"));
     client.print(F("event.preventDefault();"));
     client.print(F("let submitterId = event.submitter.id;"));
     client.print(F("console.log(\"Submit from: \", submitterId);"));
     client.print(F("const form = document.querySelector('#theForm');"));
     client.print(F("const formData = new FormData(form);"));
     client.print(F("/*for (const key of formData.keys()) {"));
     client.print(F("console.log(key);"));
     client.print(F("}"));
     client.print(F("for (const key of formData.values()) {"));
     client.print(F("console.log(key);"));
     client.print(F("}*/"));
     client.print(F("let newRequest = \"\";"));
     client.print(F("newRequest += \"IPaddr1=\" + formData.get(\"IPaddr1\") +"));
     client.print(F("\"&S11=\" + formData.get(\"S11\") +"));
     client.print(F("\"&S12=\" + formData.get(\"S12\") +"));
     client.print(F("\"&S13=\" + formData.get(\"S13\") +"));
     client.print(F("\"&S14=\" + formData.get(\"S14\") +"));
     client.print(F("\"&S15=\" + formData.get(\"S15\") +"));
     client.print(F("\"&S16=\" + formData.get(\"S16\") +"));
     client.print(F("\"&S17=\" + formData.get(\"S17\") +"));
     client.print(F("\"&S18=\" + formData.get(\"S18\");"));
     client.print(F("newRequest += \"&IPaddr2=\" + formData.get(\"IPaddr2\") +"));
     client.print(F("\"&S21=\" + formData.get(\"S21\") +"));
     client.print(F("\"&S22=\" + formData.get(\"S22\") +"));
     client.print(F("\"&S23=\" + formData.get(\"S23\") +"));
     client.print(F("\"&S24=\" + formData.get(\"S24\") +"));
     client.print(F("\"&S25=\" + formData.get(\"S25\") +"));
     client.print(F("\"&S26=\" + formData.get(\"S26\") +"));
     client.print(F("\"&S27=\" + formData.get(\"S27\") +"));
     client.print(F("\"&S28=\" + formData.get(\"S28\");"));
     client.print(F("newRequest += \"&ATEM\" +"));
     client.print(F("\"&S31=\" + formData.get(\"S31\") +"));
     client.print(F("\"&S32=\" + formData.get(\"S32\") +"));
     client.print(F("\"&S33=\" + formData.get(\"S33\") +"));
     client.print(F("\"&S34=\" + formData.get(\"S34\") +"));
     client.print(F("\"&S35=\" + formData.get(\"S35\") +"));
     client.print(F("\"&S36=\" + formData.get(\"S36\") +"));
     client.print(F("\"&S37=\" + formData.get(\"S37\") +"));
     client.print(F("\"&S38=\" + formData.get(\"S38\");"));
     client.print(F("if (formData.has(\"en1\")) {"));
     client.print(F("newRequest += \"&en1\""));
     client.print(F("}"));
     client.print(F("if (formData.has(\"en2\")) {"));
     client.print(F("newRequest += \"&en2\""));
     client.print(F("}"));
     client.print(F("if (formData.has(\"en3\")) {"));
     client.print(F("newRequest += \"&en3\""));
     client.print(F("}"));
     client.print(F("if (submitterId == 'save') {"));
     client.print(F("newRequest += \"&save\""));
     client.print(F("}"));
     client.print(F("console.log(\"Request: \", newRequest);"));
     client.print(F("window.location.replace(newRequest);"));
     client.print(F("});"));
     client.print(F("});"));
     client.print(F("</script>"));
     client.print(F("<div id=\"cover\">"));
     client.print(F("<div class=\"loader\">"));
     client.print(F("<p>Loading...</p>"));
     client.print(F("<div id=\"loader\"></div>"));
     client.print(F("</div>"));
     client.print(F("</div>"));
     client.print(F("<h2 style=\"text-align: center; margin-bottom: 0px;\">Tally Light System configuration</h2>"));
     client.print(F("<h4 style=\"text-align: center; margin-top: 0px;\">for two vMix switchers and Blackmagic ATEM over SDI</h4>"));
     client.print(F("<form action=\"\" method=\"GET\" id=\"theForm\">"));
     client.print(F("<div class=\"container\" id=\"a\">"));
     client.print(F("<table id=\"switches\">"));
     client.print(F("<td>"));
     client.print(F("<div class=\"switches\" id=\"vMix_1\">"));
     client.print(F("<h3 style=\"text-align: center;\">vMix 1</h3>"));
     client.print(F("<div class=\"switch\">"));
     client.print(F("<input id=\"switch-1\" type=\"checkbox\" class=\"switch-input\" name=\"en1\" "));
     client.print(sw1.enable);
     client.print(F(">"));
     client.print(F("<label for=\"switch-1\" class=\"switch-label\">Switch</label>"));
     client.print(F("</div><br>"));
     client.print(F("</div>"));
     client.print(F("</td>"));
     client.print(F("<td>"));
     client.print(F("<div class=\"switches\" id=\"ATEM\">"));
     client.print(F("<h3 style=\"text-align: center;\">ATEM SDI</h3>"));
     client.print(F("<div class=\"switch\">"));
     client.print(F("<input id=\"switch-3\" type=\"checkbox\" class=\"switch-input\" name=\"en3\" "));
     client.print(sw3.enable);
     client.print(F(">"));
     client.print(F("<label for=\"switch-3\" class=\"switch-label\">Switch</label>"));
     client.print(F("</div><br>"));
     client.print(F("</div>"));
     client.print(F("</td>"));
     client.print(F("<td>"));
     client.print(F("<div class=\"switches\" id=\"vMix_2\">"));
     client.print(F("<h3 style=\"text-align: center;\">vMix 2</h3>"));
     client.print(F("<div class=\"switch\">"));
     client.print(F("<input id=\"switch-2\" type=\"checkbox\" class=\"switch-input\" name=\"en2\" "));
     client.print(sw2.enable);
     client.print(F(">"));
     client.print(F("<label for=\"switch-2\" class=\"switch-label\">Switch</label>"));
     client.print(F("</div><br>"));
     client.print(F("</div>"));
     client.print(F("</td>"));
     client.print(F("</table>"));
     client.print(F("<div class=\"switchers\" id=\"vMix_1\">"));
     client.print(F(""));
     client.print(status1);
     client.print(F(""));
     client.print(F("<label for=\"IPaddr\">IP address</label>"));
     client.print(F("<input type=\"text\" id=\"IPaddr\" name=\"IPaddr1\" placeholder=\"IP address\""));
     client.print(F("value=\""));
     client.print(sw1.IPaddr[0]);
     client.print(".");
     client.print(sw1.IPaddr[1]);
     client.print(".");
     client.print(sw1.IPaddr[2]);
     client.print(".");
     client.print(sw1.IPaddr[3]);
     client.print(F("\">"));
     client.print(F("</div>"));
     client.print(F("<div class=\"switchers\" id=\"vMix_2\">"));
     client.print(F(""));
     client.print(status2);
     client.print(F(""));
     client.print(F("<label for=\"IPaddr\">IP address</label>"));
     client.print(F("<input type=\"text\" id=\"IPaddr\" name=\"IPaddr2\" placeholder=\"IP address\""));
     client.print(F("value=\""));
     client.print(sw2.IPaddr[0]);
     client.print(".");
     client.print(sw2.IPaddr[1]);
     client.print(".");
     client.print(sw2.IPaddr[2]);
     client.print(".");
     client.print(sw2.IPaddr[3]);
     client.print(F("\">"));
     client.print(F("</div>"));
     client.print(F("<table id=\"channels\">"));
     client.print(F("<tr id=\"title\">"));
     client.print(F("<th colspan=\"4\">vMix input number setup</th>"));
     client.print(F("</tr>"));
     client.print(F("<tr>"));
     client.print(F("<th id=\"firstCol\"></th>"));
     client.print(F("<th>vMix 1</th>"));
     client.print(F("<th>vMix 2</th>"));
     client.print(F("<th>ATEM</th>"));
     client.print(F("</tr>"));
     client.print(F("<tr>"));
     client.print(F("<td><label for=\"T1\">1. Camera</label></td>"));
     client.print(F("<td><input type=\"number\" id=\"T1\" name=\"S11\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw1.inputNumber[0]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T1\" name=\"S21\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw2.inputNumber[0]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T1\" name=\"S31\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw3.inputNumber[0]);
     client.print(F("\"></td>"));
     client.print(F("</tr>"));
     client.print(F("<tr>"));
     client.print(F("<td><label for=\"T2\">2. Camera</label></td>"));
     client.print(F("<td><input type=\"number\" id=\"T2\" name=\"S12\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw1.inputNumber[1]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T2\" name=\"S22\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw2.inputNumber[1]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T2\" name=\"S32\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw3.inputNumber[1]);
     client.print(F("\"></td>"));
     client.print(F("</tr>"));
     client.print(F("<tr>"));
     client.print(F("<td><label for=\"T3\">3. Camera</label></td>"));
     client.print(F("<td><input type=\"number\" id=\"T3\" name=\"S13\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw1.inputNumber[2]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T3\" name=\"S23\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw2.inputNumber[2]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T3\" name=\"S33\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw3.inputNumber[2]);
     client.print(F("\"></td>"));
     client.print(F("</tr>"));
     client.print(F("<tr>"));
     client.print(F("<td><label for=\"T4\">4. Camera</label></td>"));
     client.print(F("<td><input type=\"number\" id=\"T4\" name=\"S14\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw1.inputNumber[3]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T4\" name=\"S24\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw2.inputNumber[3]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T4\" name=\"S34\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw3.inputNumber[3]);
     client.print(F("\"></td>"));
     client.print(F("</tr>"));
     client.print(F("<tr>"));
     client.print(F("<td><label for=\"T5\">5. Camera</label></td>"));
     client.print(F("<td><input type=\"number\" id=\"T5\" name=\"S15\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw1.inputNumber[4]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T5\" name=\"S25\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw2.inputNumber[4]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T5\" name=\"S35\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw3.inputNumber[4]);
     client.print(F("\"></td>"));
     client.print(F("</tr>"));
     client.print(F("<tr>"));
     client.print(F("<td><label for=\"T6\">6. Camera</label></td>"));
     client.print(F("<td><input type=\"number\" id=\"T6\" name=\"S16\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw1.inputNumber[5]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T6\" name=\"S26\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw2.inputNumber[5]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T6\" name=\"S36\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw3.inputNumber[0]);
     client.print(F("\"></td>"));
     client.print(F("</tr>"));
     client.print(F("<tr>"));
     client.print(F("<td><label for=\"T7\">7. Camera</label></td>"));
     client.print(F("<td><input type=\"number\" id=\"T7\" name=\"S17\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw1.inputNumber[6]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T7\" name=\"S27\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw2.inputNumber[6]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T7\" name=\"S37\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw3.inputNumber[6]);
     client.print(F("\"></td>"));
     client.print(F("</tr>"));
     client.print(F("<tr>"));
     client.print(F("<td><label for=\"T8\">8. Camera</label></td>"));
     client.print(F("<td><input type=\"number\" id=\"T8\" name=\"S18\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw1.inputNumber[7]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T8\" name=\"S28\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw2.inputNumber[7]);
     client.print(F("\"></td>"));
     client.print(F("<td><input type=\"number\" id=\"T8\" name=\"S38\" min=\"0\" max=\"255\" placeholder=\"vMix input number\""));
     client.print(F("value=\""));
     client.print(sw3.inputNumber[7]);
     client.print(F("\"></td>"));
     client.print(F("</tr>"));
     client.print(F("</table>"));
     client.print(F("</div>"));
     client.print(F("<div id=\"buttons\">"));
     client.print(F("<a class=\"button\" id=\"refresh\" href=\"/\">Refresh</a>"));
     client.print(F("<input class=\"button\" type=\"submit\" value=\"Submit\" id=\"submit\">"));
     client.print(F(""));
     client.print(save);
     client.print(F(""));
     client.print(F("</div>"));
     client.print(F("</form>"));
     client.print(F("<div id=\"footer\">"));
     client.print(version);
     client.print(F(" &copy eMT</div>"));
     client.print(F("</html>"));
}