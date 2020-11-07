
import de.bezier.data.sql.*;
import processing.net.*; 
import hypermedia.net.*;
import http.*;//
////FINAL!!!!!!!



SQLite db;
Server s1, s2;
UDP udp1, udp2;
SimpleHTTPServer server;//
JSONObject response;

String value;
String activity = ""; 

void setup()
{
    size( 100, 100 );
  udp1 = new UDP( this, 8100 );
  udp2 = new UDP( this, 8200 );
  udp1.log( true );
  udp2.log( true ); // <-- printout the connection activity
  udp1.listen( true );
   udp2.listen( true );
    //connecting server and db
    db = new SQLite( this, "playgrounds.db" );  
    s1 = new Server(this, 8100);
     
    s2 = new Server(this, 8200); 
  
    server = new SimpleHTTPServer(this); //
    
    if ( db.connect() )
    {
        db.query( "SELECT name as \"Name\" FROM SQLITE_MASTER where type=\"table\"" );
        
        while (db.next())
        {
            println( db.getString("Name") );
        }
        
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
  
  data = subset(data, 0, data.length);
  String message = new String( data );
  
  // print the result
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
} //saving info arduino-> DB
