<?php
// Disable error display in PHP
error_reporting(0);
ini_set('display_errors', 0);
$currentUrl = $_SERVER['REQUEST_URI'];

// التحقق من إذا ما كان الرابط يحتوي على التنسيق المطلوب
if (!preg_match('/(\?ip=\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}:\d+-(SOCKS5|SOCKS4))|(\?url=.+?(&ip=\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}:\d+-(SOCKS5|SOCKS4))?)$/', $currentUrl)) {
    // إذا لم يكن الرابط بالشكل المطلوب، يتم إعادة التوجيه لصفحة أخرى
    header('Location: input.php');
    exit;
}

// Function to fetch content via cURL using a proxy
function getContentWithProxy($url, $proxy, $proxy_type) {
    $ch = curl_init();

    // cURL settings
    curl_setopt_array($ch, [
        CURLOPT_URL => $url,
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_PROXY => $proxy,
        CURLOPT_PROXYTYPE => $proxy_type,
        CURLOPT_FOLLOWLOCATION => true,
        CURLOPT_TIMEOUT => 100,
        CURLOPT_CONNECTTIMEOUT => 100,
        CURLOPT_SSL_VERIFYPEER => false,
        CURLOPT_SSL_VERIFYHOST => false,
        CURLOPT_VERBOSE => false,
    ]);

    $response = curl_exec($ch);
    $httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);

    // If there's an error with the request
    if (curl_errno($ch) || $httpCode != 200) {
        curl_close($ch);
        return false;
    }

    curl_close($ch);
    return $response;
}

// Check if required parameters are present
if (isset($_GET['ip']) && !empty($_GET['ip']) && isset($_GET['url']) && !empty($_GET['url'])) {
    list($proxy, $proxy_type) = explode("-", $_GET['ip']);
    list($proxy_ip, $proxy_port) = explode(":", $proxy);

    // Check if the IP address is valid
    

    // Define the proxy type
    $proxy_type_constant = ($proxy_type == "SOCKS4") ? CURLPROXY_SOCKS4 : ($proxy_type == "SOCKS5" ? CURLPROXY_SOCKS5 : null);
   

    // Process the entered URL
    $new_url = filter_var($_GET['url'], FILTER_SANITIZE_URL); // Sanitize the URL

    // Check if the URL is valid
    
    // Get content via the proxy
    $response = getContentWithProxy($new_url, $proxy, $proxy_type_constant);
    if (!filter_var($proxy_ip, FILTER_VALIDATE_IP)) {
        echo("Invalid proxy IP address.");
    }
    else if (!filter_var($new_url, FILTER_VALIDATE_URL)) {
        echo("Invalid URL provided.");
    }
    else if (!$proxy_type_constant) {
        echo("Unsupported proxy type.");
    }
    // If the response is successful, display the content
    else if ($response !== false) {
        echo $response;
    } else {
        echo "An error occurred while trying to open the link via the proxy.";
    }
} else {
    // Show the search engine form if no URL is provided
echo '<div class="search-container" id="searchContainer">';
echo '<h1 id="searchTitle">Snake Engine</h1>';

echo '<script>
    // الحصول على النص
    const title = document.getElementById("searchTitle");
    const text = title.innerText;
    
    // مسح النص داخل h1
    title.innerHTML = "";
    
    // تحويل النص إلى مصفوفة من الأحرف (نضيف span للمسافات)
    const letters = text.split("").map(letter => {
        // إذا كان الحرف مسافة، نعيده كما هو
        if (letter === " ") {
            return "<span class=\"space\"> </span>";
        } else {
            return `<span>${letter}</span>`;
        }
    });

    // إضافة الأحرف المترجمة إلى داخل h1
    title.innerHTML = letters.join("");

    // تغيير اللون العشوائي لكل حرف
    function getRandomColor() {
        const letters = "0123456789ABCDEF";
        let color = "#";
        for (let i = 0; i < 6; i++) {
            color += letters[Math.floor(Math.random() * 16)];
        }
        return color;
    }

    // تحديث ألوان الأحرف كل 500 ملي ثانية
    setInterval(() => {
        const spans = title.getElementsByTagName("span");
        for (let i = 0; i < spans.length; i++) {
            if (spans[i].classList.contains("space")) {
                continue; // تجاهل المسافات
            }
            spans[i].style.color = getRandomColor();
        }
    }, 500); // تغيير اللون كل 500 ملي ثانية
</script>';


echo '<div class="icon-container">';
echo '<img src="conf/icon/snake.png" alt="Search Icon" class="search-icon">';
echo '</div>';
echo '<form method="GET" action="">';
echo '<input type="text" name="url" placeholder="Enter URL..." required>';
echo '<input type="hidden" name="ip" value="' . htmlspecialchars($_GET['ip'] ?? '') . '">';
echo '<input type="submit" value="GO">';
echo '</form>';
echo '</div>';

}
?>



<!-- CSS for styling -->
<style>
/* تصميم الصفحة */
body {
    font-family: 'Arial', sans-serif;
    background-color: #1a1a1a; /* خلفية داكنة */
    color: #e0e0e0; /* نص فاتح */
    margin: 0;
    padding: 0;
    text-align: center;
}

/* إعدادات الحاوية الرئيسية للبحث */
.search-container {
    padding: 40px;
    background-color: #333; /* خلفية داكنة */
    border-radius: 15px;
    box-shadow: 0 4px 20px rgba(0, 0, 0, 0.3); /* تأثير الظل */
    max-width: 600px;
    margin: 55px auto 0; /* تعديل الهوامش لرفع المسافة من الأعلى */
    box-sizing: border-box;
    position: relative; /* تحديد الموضع بالنسبة للعناصر الأخرى */
}

/* صورة الشعار */
.search-container img {
    width: 150px;
    margin-bottom: 20px;
    transition: transform 0.3s ease; /* تأثير التحولات */
}

.search-container img:hover {
    transform: scale(1.1); /* تكبير الصورة عند التمرير فوقها */
}

/* العنوان */
.search-container h1 {
    font-size: 24px;
    color: #fff;
    margin-bottom: 20px;
    text-transform: uppercase;
    letter-spacing: 2px; /* تباعد بين الحروف */
}

/* حقل البحث */
.search-container input[type="text"] {
    width: 80%;
    padding: 12px;
    margin: 5px;
    font-size: 18px;
    margin-bottom: 20px;
    border-radius: 10px;
    border: 1px solid #555; /* حدود داكنة */
    background-color: #444; /* خلفية داكنة للحقل */
    color: #fff; /* نص فاتح داخل الحقل */
    box-sizing: border-box;
    transition: border-color 0.3s ease;
}

.search-container input[type="text"]:focus {
    border-color: #4285f4; /* تغيير اللون عند التركيز */
}

/* زر البحث */
.search-container input[type="submit"] {
    padding: 12px 12px;
    font-size: 18px;
    background-color: #4285f4;
    color: white;
    margin: 5px;
    border-radius: 12px;
    border: none;
    cursor: pointer;
    transition: background-color 0.5s ease;
}

.search-container input[type="submit"]:hover {
    background-color: #357ae8; /* تأثير عند التمرير فوق الزر */
}

/* محرك البحث العائم */
.search-bar-draggable {
    position: fixed;
    top: 20px;
    left: 20px;
    background-color: rgba(40, 40, 40, 0.9); /* خلفية داكنة شبه شفافة */
    padding: 15px;
    border-radius: 12px;
    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3); /* تأثير الظل */
    z-index: 10000;
    cursor: move;
    transition: background-color 0.3s ease;
}

.search-bar-draggable:hover {
    background-color: rgba(40, 40, 40, 1); /* تغيير اللون عند التمرير فوقها */
}

/* حقل البحث العائم */
.search-bar-draggable input[type="text"] {
    width: 150px;
    padding: 8px;
    font-size: 16px;
    border-radius: 12px;
    border: 1px solid #555;
    background-color: #444;
    color: #fff;
}

/* زر البحث العائم */
.search-bar-draggable input[type="submit"] {
    padding: 8px 20px;
    font-size: 16px;
    background-color: #4285f4;
    color: white;
    border-radius: 12px;
    border: none;
    cursor: pointer;
    margin-left: 10px;
    transition: background-color 0.3s ease;
}

.search-bar-draggable input[type="submit"]:hover {
    background-color: #357ae8;
}
#searchTitle {
    display: inline-block;
}

#searchTitle span {
    display: inline-block;
    transition: color 0.3s ease; /* تأثير التحول السلس بين الألوان */
}

#searchTitle .space {
    display: inline-block;
    width: 10px; /* عرض المسافة بين الكلمات */
}

</style>

<!-- Floating Search Bar -->
<div class="search-bar-draggable" id="searchBar">
    <form method="GET" action="">
        <input type="text" name="url" placeholder="Enter URL..." required>
        <input type="hidden" name="ip" value="<?php echo htmlspecialchars($_GET['ip']); ?>">
        <input type="submit" value="Search">
    </form>
</div>
<script>
   // جعل شريط البحث العائم قابل للسحب
let searchBar = document.getElementById('searchBar');
let isDragging = false;
let offsetX, offsetY;

searchBar.addEventListener('mousedown', function (e) {
    isDragging = true;
    offsetX = e.clientX - searchBar.getBoundingClientRect().left;
    offsetY = e.clientY - searchBar.getBoundingClientRect().top;
});

window.addEventListener('mousemove', function (e) {
    if (isDragging) {
        let x = e.clientX - offsetX;
        let y = e.clientY - offsetY;
        searchBar.style.left = x + 'px';
        searchBar.style.top = y + 'px';
    }
});

window.addEventListener('mouseup', function () {
    isDragging = false;
});

// عرض شريط البحث العائم فقط عندما يتم إرسال URL
<?php if(isset($_GET['url']) && !empty($_GET['url'])): ?>
    document.getElementById('searchBar').style.display = 'block';
<?php else: ?>
    document.getElementById('searchBar').style.display = 'none';
<?php endif; ?>
// Check if the 'url' parameter is present and process it
<?php 
$baseUrl = '';
if (isset($_GET['url'])) {
    $url = $_GET['url'];
    
    // Parse the URL and extract the scheme (http/https) and host
    $parsedUrl = parse_url($url);
    
    // Rebuild the base URL with scheme and host
    if (isset($parsedUrl['scheme']) && isset($parsedUrl['host'])) {
        $baseUrl = $parsedUrl['scheme'] . '://' . $parsedUrl['host'];
    }
}
?>
document.querySelectorAll('a').forEach(function(link) {
    link.addEventListener('click', function(e) {
        e.preventDefault(); // منع الفتح الافتراضي للرابط

        let proxy = '<?php echo htmlspecialchars($_GET['ip']); ?>'; // استخدام البروكسي الحالي
        let url = this.href; // الحصول على الرابط
        let afterDomain = new URL(url).pathname; // يعطي '/path/to/page'
        // استخراج الدومين من الرابط الذي تم فتحه أولاً
        let domain = '<?php echo htmlspecialchars($baseUrl); ?>';


        // بناء الرابط الجديد مع معلمة url
        let redirectUrl = window.location.origin + '/proxy.php/?url=' + encodeURIComponent(domain+afterDomain) + '&ip=' + encodeURIComponent(proxy);

        // توجيه المستخدم إلى الرابط الجديد
        window.location.replace(redirectUrl);
    });
});





</script>
