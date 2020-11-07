// fjenett 20081129
import de.bezier.data.sql.*;
import processing.net.*; //
import hypermedia.net.*;



SQLite db;
Server s;
UDP udp;
JSONObject response;

String value;
String activity = ""; 

void setup()
{
    size( 100, 100 );
  udp = new UDP( this, 8100 );
  udp.log( true );     // <-- printout the connection activity
  udp.listen( true );
    //connecting server and db
    db = new SQLite( this, "C:/Users/ksr10/OneDrive/Documents/Processing/libraries/BezierSQLib/examples/SQLite_example1/data" );  // open database file
    s = new Server(this, 8100); // 12345 port open
     
 
    if ( db.connect() )
    {
        // list table names
        db.query( "SELECT name as \"Name\" FROM SQLITE_MASTER where type=\"table\"" );
        
        while (db.next())
        {
            println( db.getString("Name") );
        }
        
      //  db.query("insert into playground_slide(playgroundId, slideId) values(1,1)");
       // db.query("insert into playground_slide(playgroundId, slideId) values(2,2)");
        db.query( "SELECT * FROM playground_slide" );

        
        while (db.next())
        {
            println( db.getString("playgroundId") );
            println( db.getInt("slideId") );
        }
    }
}
void draw(){;}


void receive( byte[] data, String ip, int port ) {  // <-- extended handler
  
  
  // get the "real" message =
  // forget the ";\n" at the end <-- !!! only for a communication with Pd !!!
  data = subset(data, 0, data.length);
  String message = new String( data );
  
  // print the result
  println( "receive: \""+message+"\" from "+ip+" on port "+port );
  String dbString[] = message.split("/");
  
  for(int i=0;i<dbString.length;i++){
    println(dbString[i]); //
  }
  
  //db.query("insert into slides(\"slides_id\", \"case\", \"startTime\", \"endTime\") values("+dbString[0]+", "+dbString[1]+", \""+dbString[2]+"\", \""+dbString[3]+"\");");
 db.query("insert into swing(\"swing_id\", \"case\", \"startTime\", \"day\", \"duration\" ) values("+dbString[0]+", " +dbString[1]+  ", \""  +dbString[2]+  "\", \""  +dbString[3]+ "\", "+dbString[4]+");");
  int str0 = Integer.parseInt(dbString[0]);
  int str1 =Integer.parseInt(dbString[1]);
  int str3 = Integer.parseInt(dbString[3]);
  int str4 =Integer.parseInt(dbString[4]);
  //db.query("insert into swing(\"swing_id\", \"case\", \"startTime\", \"day\", \"duration\" ) values("+str0+", " +str1+  ", \""  +dbString[2]+  "\", "  +str3+ ", "+str4+");");
  //message
  //response = loadJSONObject(message);
  //response = (JSONObject) message;
  //int playname=response.getInt("playname");
 // println(playname);
  //println(response);
}
