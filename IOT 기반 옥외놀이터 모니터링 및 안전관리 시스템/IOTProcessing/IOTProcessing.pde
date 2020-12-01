// fjenett 20081129
import de.bezier.data.sql.*;
import processing.net.*; //
import hypermedia.net.*;
import http.*;
///////////////////////////////////////FINAL!!!!!!!!!!!!

SQLite db;
Server s1, s2;
SimpleHTTPServer server;
UDP udp1, udp2, udp3, udp4;
JSONObject response;
JSONArray jsonHTML;
JSONArray jsonCaseHTML;
String value;
String activity = ""; 

int red1 = 255;
int green1 =251;
int blue1 =150;

int red2 = 255;
int green2 =251;
int blue2 =150;

int red3 = 255;
int green3 =8;
int blue3 =74;

int red4 = 255;
int green4 =8;
int blue4 =74;

int red5 = 255;
int green5 =8;
int blue5 =74;

int red6 = 255;
int green6 =8;
int blue6 =74;

void setup()
{
    size( 100, 100 );
  udp1 = new UDP( this, 8100 );
  udp2 = new UDP( this, 8200 );
  udp3 = new UDP( this, 8300 );
  udp4 = new UDP( this, 8400 );
  udp1.log( true );
  udp2.log( true ); // <-- printout the connection activity
  udp3.log( true );
  udp4.log( true );
  udp1.listen( true );
  udp2.listen( true );
  udp3.listen( true );
  udp4.listen( true );
    db = new SQLite( this, "playgrounds.db" );  // open database file
    s1 = new Server(this, 8100); // 12345 port open
    s2 = new Server(this, 8200); // 12345 port open
    server = new SimpleHTTPServer(this);
    jsonHTML= new JSONArray();
    jsonCaseHTML=new JSONArray();
    if ( db.connect() )
    {
        int i=0;
        db.query( "SELECT \"case\", count(id) as cnt FROM slides group by \"case\"" );
        while (db.next())
        {
         //println( db.getString("case") +" ");
         int cnts=db.getInt("cnt");
         jsonCaseHTML.setInt(i++,cnts);
        }
        println();
 
        String ddate;
        int z=0;
        int y=0;
        int[] array = new int[1001];
        int[][] days = new int[7][24];
        i=0;
        db.query("SELECT * FROM day_time");
        while(db.next()){
            
          //println(db.getString("day") +" "+db.getString("time")+" "+db.getString("value"));
          array[i]=db.getInt("value");
          jsonHTML.setInt(i,array[i]/4);
          println(array[i++]);
        }
         println("now i:"+i); 
        /*for(int i=1;i<=28;i++){
          for(int j=0;j<=23;j++){
            ddate = "2020-11-"+i+"-"+j+"-";
            db.query("SELECT count(id) as cnt FROM slides where startTime like \""+ddate+"%\"" );
       
            while (db.next())
            {
              array[z] = Integer.parseInt(db.getString("cnt"));
              days[(i-1)%7][(y++)%24]+= array[z++];
            }
          }}
          println();
          for(int a=0 ; a<7;a++){
            for(int b=0;b<24;b++){
              
            print(days[a][b]+" ");
            String ttime=b+":00~"+(b+1)+":00";
            println(ttime);
            
            db.query("insert into day_time(\"day\", \"time\", \"value\") values("+a+", \""+ttime+"\", "+days[a][b]+");");
            }
            println();
          }*/
    }
    print(jsonHTML);
    DynamicResponseHandler responder = new DynamicResponseHandler(new JSONEcho(), "application/json");
    DynamicResponseHandler responder2 = new DynamicResponseHandler(new JSONEcho2(), "application/json2");
    server.serveAll("");
    server.serveAll("./htmlfile");
    server.createContext("htmlfile/echo", responder);
    server.createContext("htmlfile/echo2", responder2);
}
void draw(){;}


void receive( byte[] data, String ip, int port ) {  
 
  data = subset(data, 0, data.length);
  String message = new String( data );

  if(port==8100){
  println( "receive: \""+message+"\" from "+ip+" on port "+port );
  String dbString[] = message.split("/");
  
  for(int i=0;i<dbString.length;i++){
    println(dbString[i]); //
  }
  
  db.query("insert into slides(\"slides_id\", \"case\", \"startTime\", \"day\", \"duration\" ) values("+dbString[0]+", " +dbString[1]+  ", \""  +dbString[2]+  "\", \""  +dbString[3]+ "\", "+dbString[4]+");");
  int str0 = Integer.parseInt(dbString[0]);
  int str1 =Integer.parseInt(dbString[1]);
  int str3 = Integer.parseInt(dbString[3]);
  int str4 =Integer.parseInt(dbString[4]);
  }
  if(port==8200){
  println( "receive: \""+message+"\" from "+ip+" on port "+port );
  String dbString[] = message.split("/");
  
  for(int i=0;i<dbString.length;i++){
    println(dbString[i]); //
  }
  
  db.query("insert into swing(\"swing_id\", \"case\", \"startTime\", \"day\", \"duration\" ) values("+dbString[0]+", " +dbString[1]+  ", \""  +dbString[2]+  "\", \""  +dbString[3]+ "\", "+dbString[4]+");");
  int str0 = Integer.parseInt(dbString[0]);
  int str1 =Integer.parseInt(dbString[1]);
  int str3 = Integer.parseInt(dbString[3]);
  int str4 =Integer.parseInt(dbString[4]);
  }
  
  if(port==8300){
    println( "receive: \""+message+"\" from "+ip+" on port "+port );
  String dbString[] = message.split("/");
 
  if(dbString[0].equals("1")){
    draw1(3);
    
  }if(dbString[1].equals("1")){
     draw1(4);
    
  }if(dbString[2].equals("1")){
     draw1(5);
    
  }if(dbString[3].equals("1")){
     draw1(6);
  }
  if(dbString[0].equals("0")){
    draw2(3);
    
  }if(dbString[1].equals("0")){
     draw2(4);
    
  }if(dbString[2].equals("0")){
     draw2(5);
    
  }if(dbString[3].equals("0")){
     draw2(6);
  }
  }
  if(port==8400){
    println( "receive: \""+message+"\" from "+ip+" on port "+port );
    String dbString[] = message.split("/");
 
    if(dbString[0].equals("1")){
      draw1(1);
    }
    else{
      draw2(1);
    }
    if(dbString[1].equals("1")){
       draw1(2);
    }
    else{
      draw2(2);
    }
  }
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
     
     case 3:
      red3 = 255;//(255,111,105)(252,52,104)255,8,74
      green3 = 8;
      blue3 = 74;
      break;
      
      case 4:
      red4 = 255;//(255,111,105)(252,52,104)255,8,74
      green4 = 8;
      blue4 = 74;
      break;
      
      case 5:
      red5 = 255;//(255,111,105)(252,52,104)255,8,74
      green5 = 8;
      blue5 = 74;
      break;
      
      case 6:
      red6 = 255;//(255,111,105)(252,52,104)255,8,74
      green6 = 8;
      blue6 = 74;
      break;
  }
}
void draw2(int num){

  switch(num){
    
     case 1:
      red1 = 255;//(255,111,105)(252,52,104)255,8,74
      green1 = 251;
      blue1 = 150;
      break;
      
      case 2:
      red2 = 255;//(255,111,105)(252,52,104)255,8,74
      green2 = 251;
      blue2 = 150;
      break;
      
     case 3:
      red3 = 255;//(255,111,105)(252,52,104)255,8,74
      green3 = 251;
      blue3 = 150;
      break;
      
      case 4:
      red4 = 255;//(255,111,105)(252,52,104)255,8,74
      green4 = 251;
      blue4 = 150;
      break;
      
      case 5:
      red5 = 255;//(255,111,105)(252,52,104)255,8,74
      green5 = 251;
      blue5 = 150;
      break;
      
      case 6:
      red6 = 255;//(255,111,105)(252,52,104)255,8,74
      green6 = 251;
      blue6 = 150;
      break;
  }
}
class JSONEcho extends ResponseBuilder {
  public  String getResponse(String requestBody) {
    JSONObject json=new JSONObject();
    json.setJSONArray("timeAVG",jsonHTML);
    json.setJSONArray("caseAVG",jsonCaseHTML);
    return json.toString();
  }
}
class JSONEcho2 extends ResponseBuilder {
  public  String getResponse(String requestBody) {
    JSONObject json=new JSONObject();
    JSONArray rgb1=new JSONArray();
    rgb1.setInt(0,red1);
    rgb1.setInt(1,green1);
    rgb1.setInt(2,blue1);
    JSONArray rgb2=new JSONArray();
    rgb2.setInt(0,red2);
    rgb2.setInt(1,green2);
    rgb2.setInt(2,blue2);
    JSONArray rgb3=new JSONArray();
    rgb3.setInt(0,red3);
    rgb3.setInt(1,green3);
    rgb3.setInt(2,blue3);
    JSONArray rgb4=new JSONArray();
    rgb4.setInt(0,red4);
    rgb4.setInt(1,green4);
    rgb4.setInt(2,blue4);
    JSONArray rgb5=new JSONArray();
    rgb5.setInt(0,red5);
    rgb5.setInt(1,green5);
    rgb5.setInt(2,blue5);
    JSONArray rgb6=new JSONArray();
    rgb6.setInt(0,red6);
    rgb6.setInt(1,green6);
    rgb6.setInt(2,blue6);
    
    json.setJSONArray("rgb1",rgb1);
    json.setJSONArray("rgb2",rgb2);
    json.setJSONArray("rgb3",rgb3);
    json.setJSONArray("rgb4",rgb4);
    json.setJSONArray("rgb5",rgb5);
    json.setJSONArray("rgb6",rgb6);
    return json.toString();

  }
}
/*

  if(dbString[0].equals("0")&& dbString[1].equals("0") 
  && dbString[2].equals("0")&&dbString[3].equals("0")){
    draw1(0);
case 0:
       red3 = 255;
       green3 =251;
       blue3 =150;

      red4 = 255;
      green4 =251;
      blue4 =150;

     red5 = 255;
     green5 =251;
     blue5 =150;

    red6 = 255;
    green6 =251;
    blue6 =150;
    break;
    
case 0:
       red3 = 255;
       green3 =251;
       blue3 =150;

      red4 = 255;
      green4 =251;
      blue4 =150;

     red5 = 255;
     green5 =251;
     blue5 =150;

    red6 = 255;
    green6 =251;
    blue6 =150;
    break;
    */
