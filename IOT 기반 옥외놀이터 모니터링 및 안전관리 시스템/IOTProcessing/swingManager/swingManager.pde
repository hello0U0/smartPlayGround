import de.bezier.data.sql.*;
import processing.net.*; //
import hypermedia.net.*;

UDP udp;
JSONObject response;
Server s;

int red1 = 255;
int green1 =251;
int blue1 =150;

int red2 = 255;
int green2 =251;
int blue2 =150;

void setup()
{
    size( 1000, 800 );
  udp = new UDP( this, 8400 );
  udp.log( true );     // <-- printout the connection activity
  udp.listen( true );
   s = new Server(this, 8400);
}


void draw(){
  
  fill(255, 158, 44); //dark yellow
  rect(400,60,200,20);
  
  //fill(255, 8, 74);
  fill(red1, green1, blue1);
  ellipse(500,250,30,30);
  fill(255, 8, 74);
  //fill(red2, green2, blue2);
  ellipse(340,170,30,30);
  
  fill(255, 158, 44);
  translate(width/2, height/2);
  rotate(PI/7.0);
  rect(-150, -290, 20, 710);
  
  fill(255, 158, 44);
  translate(width/2, height/2);
  rotate(6.0* PI/8.50);
  rect(-180, 210, 20, 710);
}

void draw1(int num){

  switch(num){
     case 1:
      red1 = 255;//(255,111,105)(252,52,104)255,8,74
      green1 = 8;
      blue1 = 74;
      break;
      
      case 2:
      red2 = 255;//(255,111,105)(252,52,104)255,8,74
      green2 = 8;
      blue2 = 74;
      break;
  }
}

void giveString(){
  
  String str = "0/0";
  String str1 = "0/1";
  String str2 = "1/0";
 
  String dbStringg[] = str2.split("/");
  for(int i=0;i<dbStringg.length;i++){
    
  println(dbStringg[i]);}
  
  if(dbStringg[0].equals("1")){
    println("ok");
    draw1(1);
    
  }if(dbStringg[1].equals("1")){
    println("ok");
     draw1(2);
    
  }
  
}

void receive( byte[] data, String ip, int port ) { 
  
  data = subset(data, 0, data.length);
  String message = new String( data ); //format : 0/1/0/1
  
  println( "receive: \""+message+"\" from "+ip+" on port "+port );
  String dbString[] = message.split("/");
 
  if(dbString[0].equals("1")){
    draw1(1);
    
  }if(dbString[1].equals("1")){
     draw1(2);
    
  }
  
  
  //db.query("insert into slides(\"slides_id\", \"case\", \"startTime\", \"endTime\") values("+dbString[0]+", "+dbString[1]+", \""+dbString[2]+"\", \""+dbString[3]+"\");");
  //message
  //response = loadJSONObject(message);
  //response = (JSONObject) message;
  //int playname=response.getInt("playname");
 // println(playname);
  //println(response);
}
