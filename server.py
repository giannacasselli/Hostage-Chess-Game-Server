# CIS 2750
# Assignment 2
# Gianna Casselli
# 1083289
# October 22, 2024
# Code based on server.py - CIS2750 F24 Lab 2

import sys; # used to get argv
import cgi; # used to parse Mutlipart FormData 
            # this should be replace with multipart in the future
import hclib;
import sqlite3;
import json;
import time;
from urllib.parse import unquote

# web server parts
from http.server import HTTPServer, BaseHTTPRequestHandler;

# used to parse the URL and extract form data for GET requests
from urllib.parse import urlparse, parse_qsl, parse_qs;
from time import strftime
from time import gmtime
from datetime import datetime
# from lab 2
# handler for our web-server - handles both GET and POST requests
GAME_TIME = 300
class MyHandler( BaseHTTPRequestHandler ):
    def do_GET(self):
        # parse the URL to get the path and form data
        parsed  = urlparse( self.path );

        # check if the web-pages matches the list
        if parsed.path in [ '/', '/index.html']:
            file_path = './index.html' if parsed.path == '/' else '.' + parsed.path
            fp = open(file_path,'r');
            content = fp.read();
             # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            self.wfile.write( bytes( content, "utf-8" ) );
            fp.close();
        elif parsed.path in [ '/start.html' ]:

            # retreive the HTML file
            fp = open( '.'+self.path );
            content = fp.read();

            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the broswer
            self.wfile.write( bytes( content, "utf-8" ) );
            fp.close();
        # necessary for piece images
        elif parsed.path.startswith("/img/"):
            file_path = '.' + parsed.path 
            self.send_response(200)
            self.send_header("Content-type", "image/png")
            self.end_headers()
            with open(file_path, 'rb') as file:
                    self.wfile.write(file.read())

        elif parsed.path.startswith("/wizard"):
            file_path = '.' + parsed.path 
            self.send_response(200)
            self.send_header("Content-type", "image/png")
            self.end_headers()
            with open(file_path, 'rb') as file:
                    self.wfile.write(file.read())

        elif parsed.path.startswith("/Hedwig"):
            file_path = '.' + parsed.path 
            self.send_response(200)
            self.send_header("Content-type", "audio/mpeg")
            self.end_headers()
            with open(file_path, 'rb') as file:
                    self.wfile.write(file.read())

        # check if the web-pages matches the list
        elif parsed.path in [ '/upload.html' ]:

            fp = open('.'+self.path );
            content = fp.read();
             # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the broswer
            self.wfile.write( bytes( content, "utf-8" ) );
            fp.close();

        elif parsed.path.startswith("/js/"):
            fp = open('.'+self.path );
            content = fp.read();
             # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "application/javascript" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            self.wfile.write( bytes( content, "utf-8" ) );
            fp.close();

        elif parsed.path.startswith("/css/"):
            fp = open('.'+self.path );
            content = fp.read();
             # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/css" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            self.wfile.write( bytes( content, "utf-8" ) );
            fp.close();

        elif parsed.path.startswith("/check_opponent"):
            query = parse_qs(parsed.query)
            game_no = query.get('game_no', [None])[0]

            if not game_no:
                self.send_response(400)  # Bad Request
                self.end_headers()
                self.wfile.write(b"Missing game_no parameter")
                return

            conn = sqlite3.connect("hc.db")
            cursor = conn.cursor()
            cursor.execute("SELECT BLACK_HANDLE FROM Game WHERE GAME_NO = ?;", (game_no,))
            row = cursor.fetchone()
            conn.close()

            # Respond with the BLACK_HANDLE if it exists
            if row and row[0]:
                content = {"black_handle": row[0]}
            else:
                content = {"black_handle": None}

            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "application/json" );
            json_content = json.dumps(content);
            self.send_header("Content-length", len(json_content));
            self.end_headers();

            self.wfile.write(bytes(json.dumps(content), "utf-8"));
        
        elif parsed.path.startswith('/gameover.html'):
            query = parse_qs(parsed.query)
            turn = query.get('turn', [None])[0]
            if(turn == 'White'):
                winner = 'Black'
            else:
                winner = 'White'
            if not turn:
                self.send_response(400)  # Bad Request
                self.end_headers()
                self.wfile.write(b"Missing game_no parameter")
                return
            content = f"""
            <!doctype html>
            <html>
            <head>
                <title>Game Over</title>
                <link rel="stylesheet" href="css/chessboard-1.0.0.css">
                <style>
                    body {{
                        background-image: url("wizard.jpg"); 
                        background-size: cover;
                        color: white; 
                        font-family: 'Cinzel', serif; 
                    }}
                </style>
            </head>
            <body>
                <h1>{winner} wins! {turn} made an invalid move.</h1>
                <p><a href="/history.html">View History</a></p>
                <p><a href="/index.html">Start New Game</a></p>
            </body>
            </html>
            """
            self.send_response(200)  # OK
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))

        elif parsed.path.startswith('/player.html'):
            query = parse_qs(parsed.query)
            game_no = query.get('game_no', [None])[0]
            turn_no = query.get('turn_no', [None])[0]
            
            game_over_flag = 0
            content1 = ""

            if not game_no or not turn_no:
                self.send_response(400)
                self.end_headers()
                self.wfile.write(b"Missing game_no or turn_no parameter")
                return

            conn = sqlite3.connect("hc.db")
            cursor = conn.cursor()  
            # Check if it's a new game (first turn)
            if turn_no == '0':
                # Starting position for a chessboard
                turn = 'White'
                board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
                real_time = time.time()
                white_time = 300  
                black_time = 300  

                # Add the starting position to the Boards table if it doesn't exist
                cursor.execute("""
                    INSERT OR IGNORE INTO Board (GAME_NO, TURN_NO, TURN, BOARD, REAL_TIME, WHITE_TIME, BLACK_TIME)
                    VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP, ?, ?);
                """, (game_no, turn_no, turn, board, white_time, black_time))
                conn.commit()
            else:
                # Fetch the board state from the database for the given game and turn
                cursor.execute("""
                    SELECT TURN, BOARD, REAL_TIME, WHITE_TIME, BLACK_TIME
                    FROM Board
                    WHERE GAME_NO = ? AND TURN_NO = ?;
                """, (game_no, turn_no))
                row = cursor.fetchone()

                if not row:
                    self.send_response(404)  # Not Found
                    self.end_headers()
                    self.wfile.write(b"Board state not found")
                    return

                turn, board, real_time, white_time, black_time = row
                print(board)
                cursor.execute("""
                    SELECT RESULT FROM GAME
                    WHERE GAME_NO = ?;
                """, (game_no,))
                result_row = cursor.fetchone()
                if result_row and result_row[0]:
                    print("got result")
                    content1 = f"""
                    <!doctype html>
                    <html>
                    <head>
                        <title>Game Over</title>
                        <link rel="stylesheet" href="css/chessboard-1.0.0.css">
                        <style>
                            body {{
                                background-image: url("wizard.jpg"); 
                                background-size: cover;
                                color: white; 
                                font-family: 'Cinzel', serif; 
                            }}
                        </style>
                    </head>
                    <body>
                        <h1>{result_row[0]}</h1>
                        <p><a href="/history.html">View History</a></p>
                        <p><a href="/index.html">Start New Game</a></p>
                    </body>
                    </html>
                    """
            conn.close()


            content2 = f"""
            <!doctype html>
            <html>
            <head>
                <title>Player Board</title>
                <link rel="stylesheet" href="css/chessboard-1.0.0.css">
                <style>
                    body {{
                        background-image: url("wizard.jpg"); 
                        background-size: cover;
                        color: white; 
                        font-family: 'Cinzel', serif; 
                    }}
                    #boardContainer {{
                        display: flex;
                        justify-content: center;
                        align-items: center;
                        margin: 0 auto;
                        width: 100%;
                        max-width: 400px;
                    }}
                </style>
                <script src="http://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
                <script src="js/chessboard-1.0.0.js"></script>
                <script src="https://cdnjs.cloudflare.com/ajax/libs/chess.js/0.10.3/chess.min.js" integrity="sha512-xRllwz2gdZciIB+AkEbeq+gVhX8VB8XsfqeFbUh+SzHlN96dEduwtTuVuc2u9EROlmW9+yhRlxjif66ORpsgVA==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
                <script>
                    var game_no = {game_no};
                    var turn_no = {turn_no};
                    var turn = "{turn}";
                    var white_time = {white_time};
                    var black_time = {black_time};
                    var chess = new Chess()

                    function mmss(secs) {{
                        const time = new Date(secs * 1000);
                        return time.toISOString().substr(14, 5);
                    }}

                    function countdown() {{
                        if (turn === "White" && white_time > 0) {{
                            white_time--;
                            document.getElementById("white_timer").textContent = `White: ${{mmss(white_time)}}`;
                        }} else if (turn === "Black" && black_time > 0) {{
                            black_time--;
                            document.getElementById("black_timer").textContent = `Black: ${{mmss(black_time)}}`;
                        }}

                        if ((turn === "Black" && black_time <= 0) || (turn === "Black" && black_time >= 300) || (turn === "White" && white_time >= 300) || (turn === "White" && white_time <= 0)) {{
                            clearInterval(intid);
                        }}
                    }}

                    function onDone() {{
                        var boardState = document.getElementById('board_state').value;
                        var next_turn_no = parseInt(turn_no) + 1;
                        window.location.href = `/opponent.html?game_no=${{game_no}}&turn_no=${{turn_no}}&board=${{encodeURIComponent(boardState)}}`;
                    }}

                    // Initialize timers
                    document.addEventListener("DOMContentLoaded", function () {{
                        document.getElementById("white_timer").textContent = `White: ${{mmss(white_time)}}`;
                        document.getElementById("black_timer").textContent = `Black: ${{mmss(black_time)}}`;
                    }});

                    const intid = setInterval(countdown, 1000);
                    function isValid(fen, move){{
                        chess.load(fen);
                        console.log(fen);
                        var legal_moves = chess.moves({{ verbose: true, legal: false }});
                        return legal_moves.some(m => m.from + m.to === move);
                    }}

                    // Chessboard.js initialization
                    document.addEventListener("DOMContentLoaded", function () {{
                        var config = {{
                            draggable: true,
                            dropOffBoard: 'snapback',
                            position: "{board}",
                            onDrop: function (source, target, piece, newPosition, oldPosition, orientation) {{
                                var move = source + target; //Creat UCI formatted move for validation function
                                var oldfen = Chessboard.objToFen(oldPosition);
                                var col = "w";
                                var invalid = 1;
                                if(turn === "Black"){{
                                    col = "b";
                                }}
                                oldfen = oldfen + " " + col + " " + "KQkq - 0 1";

                                if(!isValid(oldfen, move)){{
                                    var boardState = document.getElementById('board_state').value;
                                    console.log("invalid move detected");
                                    window.location.href = `/opponent.html?game_no=${{game_no}}&turn_no=${{turn_no}}&board=${{encodeURIComponent(boardState)}}&valid=${{invalid}}`;
                                }}
                                chess.move({{ from: source, to: target}});
                                var fen = Chessboard.objToFen(newPosition); // Convert the new position to FEN
                                console.log("fenstr is ", fen);
                                document.getElementById('board_state').value = fen; // Update hidden input
                                console.log("Board updated after move:", fen); // Log the updated board
                            }}   
                        }};
                        var board = Chessboard('myBoard', config);
                        chess.load("{board}");

                    }});
                </script>
            </head>
            <body>
                <div id="boardContainer">
                    <div id="myBoard" style="width: 400px"></div>
                </div>
                <input type="hidden" id="board_state" value="{board}">
                <div>
                    <div id="white_timer">White: {white_time // 60}:{white_time % 60:02}</div>
                    <div id="black_timer">Black: {black_time // 60}:{black_time % 60:02}</div>
                </div>
                <button onclick="onDone()">Done</button>
            </body>
            </html>
            """
            # Send the response
            if content1:
                content = content1
            else:
                content = content2
            self.send_response(200)  # OK
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))


        elif parsed.path.startswith('/check_board'):
            query = parse_qs(parsed.query)
            game_no = query.get('game_no', [None])[0]
            turn_no = query.get('turn_no', [None])[0]

            if not game_no or not turn_no:
                self.send_response(400)  # Bad Request
                self.end_headers()
                self.wfile.write(b"Missing game_no or turn_no parameter")
                return

            conn=sqlite3.connect("hc.db")
            cursor = conn.cursor()
            nexturn = int(turn_no) 
            cursor.execute("""
                SELECT 1 FROM Board
                WHERE GAME_NO = ? AND TURN_NO = ?;
            """, (game_no,nexturn))
            row = cursor.fetchone()
            conn.close()

            if row:
                content = {"new_turn": True}
                print("true")
            else:
                content = {"new_turn": False}
                print("false")
            
            json_content = json.dumps(content)
            self.send_response(200)  # OK
            self.send_header("Content-type", "application/json")
            self.send_header("Content-length", len(json_content))
            self.end_headers()
            self.wfile.write(bytes(json_content, "utf-8"))

        elif parsed.path.startswith('/opponent.html'):
            query = parse_qs(parsed.query)
            game_no = query.get('game_no', [None])[0]
            turn_no = query.get('turn_no', [None])[0]
            board = query.get('board', [None])[0]
            invalid = query.get('valid', [None])[0]
            print("invalid num is ", invalid)

            if board:
                board = unquote(board)
                print(board)
            

            if not game_no or not turn_no:
                self.send_response(400)  # Bad Request
                self.end_headers()
                self.wfile.write(b"Missing game_no or turn_no parameter")
                return

            conn = sqlite3.connect("hc.db")
            cursor = conn.cursor()

            #if from player.html
            if board:
                game_no = int(game_no)
                turn_no = int(turn_no)
                print("turn num is", turn_no)

                # Fetch the most recent turn
                cursor.execute("""
                    SELECT TURN, REAL_TIME, WHITE_TIME, BLACK_TIME
                    FROM Board
                    WHERE GAME_NO = ? AND TURN_NO = ?;
                    """, (game_no, turn_no))
                row = cursor.fetchone()

                if not row:
                    self.send_response(404)  # Not Found
                    self.end_headers()
                    self.wfile.write(b"Previous turn not found")
                    return
                
                xturn, xrtime, xwtime, xbtime = row
                if isinstance(xrtime, str):
                    xrtime = int(time.mktime(datetime.strptime(xrtime, "%Y-%m-%d %H:%M:%S").timetuple()))
                xwtime = int(xwtime)
                xbtime = int(xbtime)
                currtime = int(time.time())
                timediff = currtime - xrtime
                new_wtime = xwtime
                new_btime = xbtime

                if xturn == 'White':
                    new_wtime = xwtime - timediff
                    new_turn = 'Black'
                else:
                    new_btime = xbtime - timediff
                    new_turn = 'White'
                result = ''
                wk = 0
                bk = 0
                for c in board:
                    if c == 'K':
                        wk = 1
                    elif c == 'k':
                        bk = 1

                if(new_wtime<0):
                    result = 'White loses on account of time. Black wins!'
                elif(new_btime<0):
                    result = 'Black loses on account of time. White wins!'
                
                if(wk == 0):
                    result = 'Black wins due to White King loss!'
                elif(bk == 0):
                    result = 'White wins due to Black King loss!'
                
                if(invalid):
                    print("invalid")
                    if(xturn == 'White'):
                        result = 'Black wins! White made an illegal move'
                    else:
                        result = 'White wins! Black made and illegal move'

                cursor.execute("""
                    INSERT INTO Board (GAME_NO, TURN_NO, TURN, BOARD, REAL_TIME, WHITE_TIME, BLACK_TIME)
                    VALUES(?, ?, ?, ?, ?, ?, ?)
                """, (game_no, turn_no+1, new_turn, board, currtime, new_wtime, new_btime))
                print("Inserted turn num into db", turn_no+1)

                if result:
                    cursor.execute("""
                    UPDATE Game
                    SET RESULT = ?
                    WHERE GAME_NO = ?
                    """, (result, game_no))
                
                conn.commit()
                if result and result[0]:
                    # Game over page
                    print("game over")
                    content = f"""
                    <!doctype html>
                    <html>
                    <head>
                        <title>Game Over</title>
                        <link rel="stylesheet" href="css/chessboard-1.0.0.css">
                        <style>
                            body {{
                                background-image: url("wizard.jpg"); 
                                background-size: cover;
                                color: white; 
                                font-family: 'Cinzel', serif; 
                            }}
                        </style>
                    </head>
                    <body>
                        <h1>{result}</h1>
                        <p><a href="/history.html">View History</a></p>
                        <p><a href="/index.html">Start New Game</a></p>
                    </body>
                    </html>
                    """
                else:
                    content = f"""
                    <!doctype html>
                    <html>
                    <head>
                        <title>Opponent's Turn</title>
                        <link rel="stylesheet" href="css/chessboard-1.0.0.css">
                        <style>
                            body {{
                                background-image: url("wizard.jpg"); 
                                background-size: cover;
                                color: white; 
                                font-family: 'Cinzel', serif; 
                            }}
                            #boardContainer {{
                                display: flex;
                                justify-content: center;
                                align-items: center;
                                margin: 0 auto;
                                width: 100%;
                                max-width: 400px;
                            }}
                        </style>
                        <script>
                            var game_no = {game_no};
                            var turn_no = {turn_no};

                            function check_new_turn(){{
                                fetch(`/check_board?game_no=${{game_no}}&turn_no=${{turn_no+2}}`)
                                    .then(response => response.json())
                                    .then(data => {{
                                        if(data.new_turn){{
                                            window.location.href = `/player.html?game_no=${{game_no}}&turn_no=${{parseInt(turn_no)+2}}`;
                                        }}
                                    }});
                            }}
                            setInterval(check_new_turn,1000);
                        </script>
                        </head>
                        <body>
                            <h1>Waiting for Opponent to Move...</h1>
                            <div id="boardContainer">
                                <div id="myBoard" style="width: 800px"></div>
                            </div>
                            <script src="http://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
                            <script src="js/chessboard-1.0.0.js"></script>
                            <script>
                                var config = {{
                                    draggable: false,
                                    dropOffBoard: 'snapback', 
                                    position: "{board}"
                                }}
                                var board = Chessboard('myBoard', config)
                            </script>
                        </body>
                        </html>
                        """
            else:
                #if from index.html
                time.sleep(2)
                cursor.execute("""
                    SELECT TURN, BOARD, WHITE_TIME, BLACK_TIME
                    FROM Board
                    WHERE GAME_NO = ? ORDER BY TURN_NO DESC LIMIT 1;
                """,(game_no,))    
                row = cursor.fetchone()
                conn.close()

                if not row:
                    self.send_response(404)  # Not Found
                    self.end_headers()
                    self.wfile.write(b"No board found for this game")
                    return
                turn, board, white_time, black_time = row
                content = f"""
                <!doctype html>
                <html>
                <head>
                    <title>Opponent's Turn</title>
                    <link rel="stylesheet" href="css/chessboard-1.0.0.css">
                    <style>
                        body {{
                            background-image: url("wizard.jpg"); 
                            background-size: cover;
                            color: white; 
                            font-family: 'Cinzel', serif; 
                        }}
                        #boardContainer {{
                            display: flex;
                            justify-content: center;
                            align-items: center;
                            margin: 0 auto;
                            width: 100%;
                            max-width: 400px;
                        }}
                    </style>
                    <script>
                        var game_no = {game_no};
                        var turn_no = {turn_no};

                        function check_new_turn(){{
                            fetch( `check_board?game_no=${{game_no}}&turn_no=${{turn_no+1}}`)
                                .then(response => response.json())
                                .then(data => {{
                                    if(data.new_turn){{
                                        window.location.href = `player.html?game_no=${{game_no}}&turn_no=${{turn_no+1}}`;
                                    }}
                                }})
                        }}
                        setInterval(check_new_turn, 1000);
                    </script>
                </head>
                <body>
                    <h1>Waiting for Opponent's Move...</h1>
                    <div id="boardContainer">
                        <div id="myBoard" style="width: 800px"></div>
                    </div>
                    <script src="http://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
                    <script src="js/chessboard-1.0.0.js"></script>
                    <script>
                        var config = {{
                            draggable: false,
                            dropOffBoard: 'snapback', 
                            position: 'start'
                        }}
                        var board = Chessboard('myBoard', config)
                    </script>
                </body>
                </html>

                """
            # Send the response
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))

        elif parsed.path.startswith('/history.html'):
            conn = sqlite3.connect("hc.db")
            cursor = conn.cursor()
            cursor.execute("SELECT GAME_NO, WHITE_HANDLE, BLACK_HANDLE, RESULT FROM Game;")
            games = cursor.fetchall()

            rows = ""
            for game in games:
                game_no, white_handle, black_handle, result = game
                rows += f"""
                <tr>
                    <td>{game_no}</td>
                    <td>{white_handle}</td>
                    <td>{black_handle or 'Waiting for Opponent'}</td>
                    <td>{result or 'In Progress'}</td>
                    <td><a href="/gamelog.html?game_no={game_no}">View Game Log</a></td>
                </tr>
                """

            content = f"""
            <!doctype html>
            <html>
            <head>
                <title>Game History</title>
                <style>
                    body {{
                        background-color: #1d313d;
                        font-family: Cinzel, serif;
                        color: white;
                        margin: 20px;
                    }}
                    table {{
                        width: 100%;
                        border-collapse: collapse;
                        margin: 20px 0;
                        background-color: #17252d;
                    }}
                    th, td {{
                        border: 1px solid #0000;
                        padding: 10px;
                        text-align: center;
                    }}
                    th {{
                        background-color: #536f81;
                    }}
                    tr:hover {{
                        background-color: #536f81;
                    }}
                </style>
            </head>
            <body>
                <h1>Game History</h1>
                <table>
                    <thead>
                        <tr>
                            <th>Game Number</th>
                            <th>White Player</th>
                            <th>Black Player</th>
                            <th>Result</th>
                            <th>Action</th>
                        </tr>
                    </thead>
                    <tbody>
                        {rows}
                    </tbody>
                </table>
            </body>
            </html>
            """
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))

        elif parsed.path.startswith('/gamelog.html'):
            query = parse_qs(parsed.query)
            game_no = query.get('game_no', [None])[0]
            if not game_no:
                self.send_response(404)  # Not Found
                self.end_headers()
                self.wfile.write(b"Game not found. ")
                return

            conn = sqlite3.connect("hc.db")
            cursor = conn.cursor()
            cursor.execute("""
                SELECT TURN_NO, TURN, BOARD, WHITE_TIME, BLACK_TIME
                FROM Board
                WHERE GAME_NO = ?
                ORDER BY TURN_NO;
            """, (game_no,))
            boards = cursor.fetchall()
            cursor.execute("""
                SELECT RESULT
                FROM Game
                WHERE GAME_NO = ?;
            """, (game_no,))
            result_row = cursor.fetchone()
            if result_row:
                result = result_row[0]
            else:
                result = "Not Available"
            
            conn.close()
            board_html = ""
            for b in boards:
                turn_no, turn, board, white_time, black_time = b
                if(white_time>300):
                    white_time = white_time - 18000
                if(turn=='White'):
                    turn = 'Black'
                else:
                    turn = 'White'
                if(turn_no == 0):
                    turn = 'Starting Board'

                board_html += f"""
                <tr>
                    <td>{turn_no}</td>
                    <td>{turn}</td>
                    <td>
                        <div id=boardContainer>
                            <div id="board{turn_no}"style="width: 400px; margin: 20px 0;"></div>
                        </div>
                        <script>
                            var config = {{
                                draggable: false,
                                position: "{board}"
                            }};
                            Chessboard('board{turn_no}', config);
                        </script>
                    </td>
                    <td>{white_time // 60}:{white_time % 60:02}</td>
                    <td>{black_time // 60}:{black_time % 60:02}</td>
                </tr>
                """
            content = f"""
            <!doctype html>
            <html>
            <head>
                <script src="http://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
                <script src="js/chessboard-1.0.0.js"></script>
                <link rel="stylesheet" href="css/chessboard-1.0.0.css">
                <title>Game Log</title>
                <style>
                    body {{
                        background-color: #1d313d;
                        font-family: Cinzel, serif;
                        color: white;
                        margin: 20px;
                    }}
                    table {{
                        width: 100%;
                        border-collapse: collapse;
                        margin: 20px 0;
                        background-color: #17252d;
                    }}
                    th, td {{
                        border: 1px solid #ffffff;
                        padding: 10px;
                        text-align: center;
                    }}
                    th {{
                        background-color: #536f81;
                    }}
                    tr:hover {{
                        background-color: #536f81;
                    }}
                    #boardContainer {{
                        display: flex;
                        justify-content: center;
                        align-items: center;
                        margin: 0 auto;
                        width: 100%;
                        max-width: 400px;
                    }}
                </style>
            </head>
            <body> 
                <h1>Game History for Game {game_no}</h1>
                <table>
                    <thead>
                        <tr>
                            <th>Turn Number</th>
                            <th>Turn</th>
                            <th>Board</th>
                            <th>White Time</th>
                            <th>Black Time</th>
                        </tr>
                    </thead>
                    <tbody>
                        {board_html}
                    </tbody>
                </table>
                <div><a href="/history.html">View Game History</a></div>
                <div><a href="/index.html">New Game</a></div>
            </body>
            </html>

            """
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))

        else:
            # generate 404 for GET requests that aren't the 3 files above
            self.send_response( 404 );
            self.end_headers();
            self.wfile.write( bytes( "404: %s not found" % self.path, "utf-8" ) );


    def do_POST(self):
        # hanle post request
        # parse the URL to get the path and form data
        parsed  = urlparse( self.path );

        # if parsed.path in [ '/display.html' ]:

        #     # get data send as Multipart FormData (MIME format)
        #     form = cgi.FieldStorage( fp=self.rfile,
        #                              headers=self.headers,
        #                              environ = { 'REQUEST_METHOD': 'POST',
        #                                          'CONTENT_TYPE': 
        #                                            self.headers['Content-Type'],
        #                                        } 
        #                            );
                                
        # #initialize content in case stringboard file cannot open/ something is wrong
        #     content = None
        #     if 'stringboard' in form:
        #         # store the file contents
        #         stringboard = form['stringboard'].file.read().decode('utf-8')
        #         #debug
        #         print("Stringboard contents: ", stringboard)

        #         # pass to exboard
        #         exboard = hclib.boardstring(stringboard)
        #         # create fen string
        #         fenstr = hclib.fen1(exboard, 'W', '-', '-', 0, 0)
        #         # print(fenstr)
        #         # pass middle part of fen string (standard fen string) to showboard function
        #         fen_parts = fenstr.split("/")  # Split by '/' to get individual rows
        #         # rejoin the segments to form standar fen string
        #         middle_fen = "/".join(fen_parts[4:12])
        #         print(fenstr)
        #         content = self.showboard(fenstr)

        #     # generate the headers
        #     self.send_response( 200 ); # OK
        #     self.send_header( "Content-type", "text/html" );
        #     self.send_header( "Content-length", len( content ) );
        #     self.end_headers();

        #     # send it to the browser
        #     self.wfile.write( bytes( content, "utf-8" ) );
        #     #fp.close();

        if parsed.path in [ '/board.html' ]:
            # get data send as Multipart FormData (MIME format)
            form = cgi.FieldStorage( fp=self.rfile,
                                     headers=self.headers,
                                     environ = { 'REQUEST_METHOD': 'POST',
                                                 'CONTENT_TYPE': 
                                                   self.headers['Content-Type'],
                                               } 
                                   );
                                
            #initialize content in case stringboard file cannot open/ something is wrong
            content = None
            if 'stringboard' in form:
                # store the file contents
                stringboard = form['stringboard'].value.decode('utf-8') if isinstance(form['stringboard'].value, bytes) else form['stringboard'].value
                turn = form.getvalue('turn', [''])[0]
                wtime = int(form.getvalue('wtime', [0])) 
                btime = int(form.getvalue('btime', [0]))  
                f_wtime = strftime("%M:%S",gmtime(wtime))
                f_btime = strftime("%M:%S",gmtime(btime))
                next_turn = ''
                if(turn == 'w' or turn == 'W'):
                    next_turn = 'b'
            
                else:
                    next_turn = 'w'
    
            content = f"""
            <!doctype html>
            <html>

            <head>
                <title>test</title>
                <link rel="stylesheet" href="css/chessboard-1.0.0.css">
                <style>
                    body {{
                        background-image: url("wizard.jpg"); 
                        background-size: cover;
                        color: white; 
                        font-family: 'Cinzel', serif; 
                    }}
                    #boardContainer {{
                        display: flex;
                        justify-content: center;
                        align-items: center;
                        margin: 0 auto;
                        width: 100%;
                        max-width: 400px;
                    }}
                </style>
            </head>


            <body>
                <div id="boardContainer">
                    <div id="myBoard" style="width: 400px"></div>
                </div>
                <audio id="music" autoplay loop>
                    <source src="Hedwig.mp3" type="audio/mpeg">
                </audio>
                <!--- Begin Example HTML ------------------------------------------------------>
                <div id="myBoard" style="width: 400px"></div>
                <!--- End Example HTML -------------------------------------------------------->
                <script src="http://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
                <script src="js/chessboard-1.0.0.js"></script>
                <script>
                    var config = {{
                        draggable: true,
                        dropOffBoard: 'snapback', // this is the default
                        position: 'start'
                    }}
                    var board = Chessboard('myBoard', config)
                </script>
                <div class = "inline">
                    <div class="inline-div" id="white_time">White: {f_wtime}</div>
                    <form action="board.html" method="post" enctype="multipart/form-data">
                        <input type="hidden" name="turn" id="t" value="{next_turn}">
                        <input type="hidden" name="wtime" id="w" value="{wtime}">
                        <input type="hidden" name="btime" id="b" value="{btime}">
                        <input type="hidden" name="stringboard" value="{stringboard}">
                        <button type="submit">Done</button>
                    </form>
                    <div class="inline-div" id="black_time">{f_btime} :Black</div>
                </div>
                <script>
                    let turn = document.getElementById('t').value;
                    let wtime = parseInt(document.getElementById('w').value);
                    let btime = parseInt(document.getElementById('b').value);

                    function mmss(secs){{
                        const time = new Date(secs*1000);
                        return time.toISOString().substr(14, 5);
                    }}
                    document.getElementById("white_time").textContent = `White: ${{mmss(wtime)}}`;
                    document.getElementById("black_time").textContent = `${{mmss(btime)}} :Black`;

                    function countdown(){{
                        if(turn==="b" && btime>0){{
                            wtime--;
                            document.getElementById("white_time").textContent = `White: ${{mmss(wtime)}}`; 
                            document.getElementById("w").value = wtime; 
                        }}
                        else if(turn==="w" && wtime>0){{
                            btime--;
                            document.getElementById("black_time").textContent = `${{mmss(btime)}} :Black`;
                            document.getElementById("b").value = btime;
                        }}
                        if((turn==="w" && btime===0)||(turn==="b" && wtime ===0)){{
                            clearInterval(intid)
                        }}

                    }}

                    const intid = setInterval(countdown, 1000);
                </script>

            </body>

            </html>

            """
            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the browser
            self.wfile.write( bytes( content, "utf-8" ) );
        
        elif parsed.path in [ '/login.html' ]:
            # get data send as Multipart FormData (MIME format)
            form = cgi.FieldStorage( fp=self.rfile,
                                     headers=self.headers,
                                     environ = { 'REQUEST_METHOD': 'POST',
                                                 'CONTENT_TYPE': 
                                                   self.headers['Content-Type'],
                                               } 
                                   );                  
            #get handle
            handle=form.getvalue('handle')
            #connect to databases
            try:
                conn = sqlite3.connect("hc.db")
                cursor = conn.cursor()
                # Create the Game table if it doesn't exist
                cursor.execute("""
                    CREATE TABLE IF NOT EXISTS Game(
                        GAME_NO INTEGER PRIMARY KEY AUTOINCREMENT,
                        WHITE_HANDLE VARCHAR(50) NOT NULL,
                        BLACK_HANDLE VARCHAR(50),
                        RESULT VARCHAR(256)
                    )
                """)
                conn.commit()
                print("Game table ensured in database.")
            except sqlite3.Error as e:
                print("Database error during initialization:", e)


            #check for any game with empty black handle
            cursor.execute("SELECT GAME_NO FROM Game WHERE BLACK_HANDLE IS NULL LIMIT 1;")
            #check if row exists
            row=cursor.fetchone()

            if row is None:
                cursor.execute("SELECT MAX(GAME_NO) FROM Game;")
                max_gamenum = cursor.fetchone()[0] or 0
                gamenum = max_gamenum + 1
                cursor.execute("INSERT INTO Game (GAME_NO, WHITE_HANDLE, BLACK_HANDLE, RESULT) VALUES (?, ?,NULL, '');", (gamenum, handle))
                conn.commit()

                #generate html response
                content = f"""
                <!doctype html>
                <html>
                <head>
                    <title>Waiting for Opponent Message</title>
                    <script>
                        var game_no = {gamenum}
                        var turn_no = 0

                        function check_opp() {{
                            fetch(`/check_opponent?game_no=${{game_no}}`)
                            .then(response=>response.json())
                            .then(data=> {{
                                if(data.black_handle) {{
                                    window.location.href = `/player.html?game_no=${{game_no}}&turn_no=${{turn_no}}`;

                                }}
                            }});
                        }}
                        setInterval(check_opp,1000);
                    </script>
                </head>
                <style>
                    body {{
                        background-image: url("wizard.jpg"); 
                        background-size: cover;
                        color: rgb(160, 152, 173); 
                        font-family: 'Cinzel', serif; 
                    }}
                </style>
                <body>
                    <h1>Waiting for Opponent...</h1>
                </body>
                </html>
                """
            else:
                print("made it!")
                gamenum = row[0]
                cursor.execute("UPDATE Game SET BLACK_HANDLE = ? WHERE GAME_NO = ?;",(handle,gamenum))
                conn.commit()

                #make a table if one does not exist
                cursor.execute("""
                CREATE TABLE IF NOT EXISTS Board(
                GAME_NO INTEGER,
                TURN_NO INTEGER,
                TURN TEXT CHECK(TURN IN ('White','Black')),
                BOARD TEXT NOT NULL,
                REAL_TIME TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                WHITE_TIME INTEGER,
                BLACK_TIME INTEGER,
                PRIMARY KEY (GAME_NO, TURN_NO),
                FOREIGN KEY (GAME_NO) REFERENCES Game(GAME_NO));
                """)
                conn.commit()
                board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
                turn_no = 0
                cursor.execute("""
                    INSERT INTO Board (GAME_NO, TURN_NO, TURN, BOARD, WHITE_TIME, BLACK_TIME)
                    VALUES (?,?,'White', ?, 300, 300);
                """, (gamenum, turn_no, board))
                conn.commit()

                #generate html content
                content = f"""
                <!doctype html>
                <html>
                <head>
                    <title>Starting Board</title>
                    <script>
                        var game_no = {gamenum}
                        var turn_no = {turn_no}
                        window.location.href=`/opponent.html?game_no=${{game_no}}&turn_no=${{turn_no}}`;
                    </script>
                </head>
                <style>
                    body {{
                        background-image: url("wizard.jpg"); 
                        background-size: cover;
                        color: rgb(160, 152, 173); 
                        font-family: 'Cinzel', serif; 
                    }}
                </style>
                <body>
                    <h1>Game Starting...</h1>
                </body>
                </html>
                """
            conn.close()
            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the browser
            self.wfile.write( bytes( content, "utf-8" ) );

       
        else:
            # generate 404 for POST requests that aren't the file above
            self.send_response( 404 );
            self.end_headers();
            self.wfile.write( bytes( "404: %s not found" % self.path, "utf-8" ) );
        

    # def showboard(self, fenstr):
    #     # function for creating html for the uploaded string board
    #     segs = fenstr.split("/")
    #     reg_fen="/".join(segs[4:12])
    #     bp = segs[0:2]
    #     ba = segs[2:4]
    #     wa = segs[12:14]
    #     temp = segs[15].split(" ")
    #     wp = ['','']
    #     wp[0] = segs[14]
    #     wp[1] = temp[0]
    #     #import jquery for chessboard.js
    #     #create grid container for each prison and airfield
    #     #each grid is 2x8
    #     #margins are set to 20px on left top and bottom
    #     #content is justified left
    #     #piece resizing in piece class
    #     #background colour setn for empty class
    #     #body html as follows
    #     board = f"""
    #     <html>
    #         <head>
    #             <title> Display Board </title>
    #             <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
    #             <link rel="stylesheet"
    #             href="https://unpkg.com/@chrisoakman/chessboardjs@1.0.0/dist/chessboard-1.0.0.min.css"
    #             integrity="sha384-q94+BZtLrkL1/ohfjR8c6L+A6qzNH9R2hBLwyoAfu3i/WCvQjzL2RQJ3uNHDISdU"
    #             crossorigin="anonymous">
    #             <script src="https://code.jquery.com/jquery-3.5.1.min.js"
    #             integrity="sha384-ZvpUoO/+PpLXR1lu4jmpXWu80pZlYUAfxl5NsBMWOEPSjUn/6Z/hRTt8+pR6L4N2"
    #             crossorigin="anonymous"></script>
    #             <script src="https://unpkg.com/@chrisoakman/chessboardjs@1.0.0/dist/chessboard-1.0.0.min.js"
    #             integrity="sha384-8Vi8VHwn3vjQ9eUHUxex3JSN/NFqUg3QbPyX8kWyb93+8AC/pPWTzj+nHtbC5bxD"
    #             crossorigin="anonymous"></script>
    #             <style>
    #                 #board {{
    #                     width: 400px;
    #                 }}
    #                 .prison, .airfield {{ display: grid; justify-content: left; grid-template-columns: repeat(8, 50px); grid-template-rows: 2fr; grid-gap: 10px; margin-left: 30px; margin-top: 20px; margin-bottom: 20px}}
    #                 .piece img {{width: 40px; height: 40px}}
    #                 .empty {{background-color: #f0f0f0; width: 50px; height: 50px}}
    #             </style>
    #         </head>
    #         <body>
    #             <h1>Chessboard</h1>
    #             <div class="prison">
    #                 <h2>Black Prison</h2>
    #                 {self.grid(bp,0)}
    #             </div>
    #             <div class="airfield">
    #                 <h2>Black Airfield</h2>
    #                 {self.grid(ba,1)}
    #             </div>

    #             <div id="board"></div>
    #             <script>
    #                 var config = {{
    #                     position: "{reg_fen}",
    #                     pieceTheme: '/img/chesspieces/wikipedia/{{piece}}.png'
    #                 }};
    #                 var board = Chessboard('board', config);
    #             </script>

    #             <div class="airfield">
    #                 <h2>White Airfield</h2>
    #                 {self.grid(wa,0)}
    #             </div>
    #             <div class="prison">
    #                 <h2>White Prison</h2>
    #                 {self.grid(wp,1)}
    #             </div>
    #         </body>
    #     </html>
    #     """
    #     return board
    
    # #funciton for adding elements to the grid
    # def grid(self, section, colour):
    #     #assign prefixes for png files
    #     if colour == 0:
    #         c = 'w'
    #     else:
    #         c = 'b'
    #     #open div
    #     html_grid = "<div class ='prison'>"
    #     for i in section:
    #         for j in i:
    #             if j.isdigit():
    #                 #iterate over every element in the section
    #                 for _ in range(int(j)):
    #                     #make empty locations 
    #                     html_grid+="<div class = 'empty'></div>"

    #             else:
    #                 #else insert pieces
    #                 p=j
    #                 if colour == 1:
    #                     p = p.upper()
    #                 piece = c+p
    #                 html_grid+=f"<div class='piece'><img src='/img/chesspieces/wikipedia/{piece}.png'></div>"
    #     #close container
    #     html_grid+="</div>"
    #     return html_grid


if __name__ == "__main__":
    httpd = HTTPServer( ( '0.0.0.0', int(sys.argv[1]) ), MyHandler );
    print( "Server listing in port:  ", int(sys.argv[1]) );
    httpd.serve_forever();
