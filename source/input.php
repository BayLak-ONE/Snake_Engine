<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Proxy Configuration</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            background: #1f1f1f;
            overflow: hidden;
        }

        .container {
            padding: 30px;
            border-radius: 20px;
            box-shadow: 0 8px 20px rgba(0, 0, 0, 0.2);
            width: 320px;
            text-align: center;
            background: linear-gradient(45deg, #ff6ec7, #ff9a8b, #fecfef, #ff7eb3);
            background-size: 400% 400%;
            animation: gradientAnimation 5s ease infinite;
            transition: transform 0.3s ease-in-out;
        }

        .container:hover {
            transform: scale(1.05);
        }

        h1 {
            font-size: 1.8em;
            margin-bottom: 20px;
            color: #fff;
        }

        label {
            display: block;
            margin-bottom: 8px;
            font-weight: bold;
            color: #fff;
        }

        input[type="text"], input[type="number"] {
            width: calc(100% - 20px);
            padding: 12px;
            margin-bottom: 15px;
            border: 2px solid #fff;
            border-radius: 8px;
            background-color: rgba(255, 255, 255, 0.2);
            color: #fff;
            font-size: 1em;
            outline: none;
            transition: all 0.3s ease;
            text-align: center;
        }

        input[type="text"]:focus, input[type="number"]:focus {
            border-color: #0072ff;
            box-shadow: 0 0 8px rgba(0, 114, 255, 0.6);
            background-color: rgba(255, 255, 255, 0.3);
        }

        .options {
            margin: 15px 0;
            color: #fff;
            text-align: left;
        }

        label input {
            margin-right: 8px;
        }

        button {
            background-color: #0072ff;
            color: white;
            border: none;
            padding: 12px 20px;
            border-radius: 8px;
            cursor: pointer;
            font-size: 1.1em;
            width: 100%;
            transition: background-color 0.3s ease, transform 0.2s ease;
        }

        button:hover {
            background-color: #0056b3;
            transform: translateY(-2px);
        }

        button:active {
            transform: translateY(0);
        }

        /* Faster animation for background gradient */
        @keyframes gradientAnimation {
            0% { background-position: 0% 50%; }
            50% { background-position: 100% 50%; }
            100% { background-position: 0% 50%; }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Proxy Configuration</h1>
        <form id="proxyForm">
            <label for="ip">IP Address</label>
            <input type="text" id="ip" name="ip" value="127.0.0.1" placeholder="Enter IP Address" required>

            <label for="port">Port</label>
            <input type="number" id="port" name="port" value="8080" placeholder="Enter Port" required>

            <div class="options">
                <label>
                    <input type="radio" name="proxy_type" value="SOCKS4" required>
                    SOCKS4
                </label>
                <label>
                    <input type="radio" name="proxy_type" value="SOCKS5" required checked>
                    SOCKS5
                </label>
            </div>

            <button type="button" onclick="submitForm()">Submit</button>
        </form>
    </div>

    <script>
        function submitForm() {
            const ip = document.getElementById('ip').value;
            const port = document.getElementById('port').value;
            const proxyType = document.querySelector('input[name="proxy_type"]:checked').value;
            const url = `/proxy.php?ip=${ip}:${port}-${proxyType}`;
            window.location.href = url;
        }
    </script>
</body>
</html>
