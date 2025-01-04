<!DOCTYPE html>
<html lang="ar">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Show Socks</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #1a1a1a;
            color: #fff;
            margin: 0;
            padding: 20px;
            line-height: 1.6;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            padding: 20px;
            background-color: #292929;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
        }
        h2 {
            color: #4caf50;
            text-align: center;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        table th, table td {
            padding: 10px;
            text-align: left;
            cursor: pointer;
        }
        table th {
            background-color: #4caf50;
            color: #fff;
        }
        table tr:nth-child(even) {
            background-color: #333;
        }
        table tr:hover {
            background-color: #4caf50;
            color: #fff;
            transition: background 0.3s ease;
        }
        .footer {
            text-align: center;
            margin-top: 20px;
            font-size: 0.9em;
            color: #bbb;
        }
    </style>
<script>
    function redirectToProxy(ipPortProtocol) {
        // توليد الرابط بدون استخدام encodeURIComponent
        const url = `/proxy.php?ip=${ipPortProtocol}`;
        // الانتقال إلى الرابط
         window.open(url, '_blank');
    }
</script>


</head>
<body>
    <div class="container">
        <?php
        // تحديد مسار الملف
        $file = 'conf/list_socks.txt';

        // التحقق من وجود الملف
        if (file_exists($file)) {
            // قراءة محتويات الملف
            $contents = file($file, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);

            // عرض المحتويات
            echo "<h2>Socks 4/5 By Snake Engine</h2>";
            echo "<table>";
            echo "<thead><tr><th>IP:PORT</th><th>Protcol</th></tr></thead>";
            echo "<tbody>";
            foreach ($contents as $line) {
                // تقسيم السطر إلى عنوان IP ونوع البروتوكول
                [$ip_port, $protocol] = explode(' - ', $line);
                $fullProxy = $ip_port . '-' . $protocol;
                echo "<tr onclick=\"redirectToProxy('$fullProxy')\">";
                echo "<td>" . htmlspecialchars($ip_port) . "</td>";
                echo "<td>" . htmlspecialchars($protocol) . "</td>";
                echo "</tr>";
            }
            echo "</tbody>";
            echo "</table>";
        } else {
            echo "<h2>No found file *list_socks.txt*.</h2>";
        }
        ?>
    </div>
    <div class="footer">
         Create In &copy; <?php echo date("Y"); ?>
    </div>
</body>
</html>
