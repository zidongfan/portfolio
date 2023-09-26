"use strict";

let wsOpen = false;

function handleConnectCB(event){
    wsOpen = true;
    console.log("ws open");
}

function handleMessageFromServerCB(event){
    let msg = event.data;
    // "{type: message, name: "Dave", room: "chatRoom", message = " Hello How are you ?"}"

    // let pieces = msg.split(" ");
    let jsonObj = JSON.parse(msg);
    let type = jsonObj.type;
    let user = jsonObj.user;
    let room = jsonObj.room;
    let realMsg = jsonObj.message;

    let msgDiv = document.getElementById("msg");
    let msgPar = document.createElement("p");

    let userDiv = document.getElementById("user");
    let userPar = document.createElement("p");
    userPar.id = user;
    userPar.innerText = user;

    // { "type": "join", "room": "chatroom", "user": "Zidong" }
    if (type == "join"){
        userDiv.appendChild(userPar);
        msgPar.innerText = user + " has entered the room.";
    }
    //{ "type": "message", "user": "Zidong", "room": "chatroom", "message": "one msg" }
    else if (type == "message"){
        msgPar.innerText = user + ": " + realMsg;

    }
    //{ "type": "leave", "room": "chatroom", "user": "Zidong" }
    else if (type == "leave"){
        document.getElementById(user).remove();
        msgPar.innerText = user + " has left the room.";
    }


    msgDiv.appendChild(msgPar);
}

function handleCloseCB(event){
    wsOpen = false;
}



function handleKeyPressCB(event){
    console.log("handle key press ..")
    if (event.keyCode == 13){

        event.preventDefault();
        if (nameTA.value == ""){
            alert("Please enter your name")
            nameTA.value = "<Enter your Name>";
            nameTA.select();
            return;
        }

        // check space and upper case
        if (roomTA.value !== roomTA.value.toLowerCase() || roomTA.value.indexOf(" ") != -1){
            alert("Room name <" + roomTA.value + "> is not valid. Room names must be all lowercase letters with no spaces" )
            roomTA.value = "<Enter a Room Name>";
            roomTA.select();
            return;
        }

        // when enter the name and room name, send request... to ws
        // Make WebSocket request to get msg..
        if (wsOpen){
            ws.send("join" + " " + nameTA.value + " " + roomTA.value);
        }
        else {
            let msgDiv = document.getElementById("msg");
            let myPar = document.createElement("p");
            myPar.innerText = "WebSocket is not open"
            msgDiv.appendChild(myPar);
        }

    }
}

function handleSendMsgCB(event){
    console.log("handle send msg...");
    if (event.keyCode == 13){
        event.preventDefault();
        if (msgSendTA.value == ""){
            alert("Please enter a validate message")
            msgSendTA.value = "<Enter a Message>";
            msgSendTA.select();
            return;
        }

        if (wsOpen){
            ws.send(nameTA.value + " " + msgSendTA.value);
            msgSendTA.value = "";
        }
        else {
            let msgDiv = document.getElementById("msg");
            let myPar = document.createElement("p");
            myPar.innerText = "WebSocket is not open"
            msgDiv.appendChild(myPar);
        }
    }

}


let nameTA = document.getElementById("nameTA");
let roomTA = document.getElementById("roomTA");
let msgSendTA = document.getElementById("msgSendTA");

nameTA.addEventListener("keypress", handleKeyPressCB);
roomTA.addEventListener("keypress", handleKeyPressCB);
msgSendTA.addEventListener("keypress",handleSendMsgCB);


// Create the websocket
let ws = new WebSocket("ws://localhost:8080");
ws.onopen = handleConnectCB;
ws.onmessage = handleMessageFromServerCB;
ws.onclose = handleCloseCB;

