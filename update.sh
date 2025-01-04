#!/bin/bash

# عنوان المستودع على GitHub
REPO_URL="https://github.com/BayLak-ONE/servers-open-port-tester-metasploit.git"

# الحصول على المسار الحالي للمجلد الذي يتم تشغيل السكربت فيه
CURRENT_DIR=$(pwd)

# تحقق إذا كان المجلد يحتوي بالفعل على مستودع Git
if [ -d ".git" ]; then
    echo "المستودع موجود بالفعل. سيتم سحب التحديثات بدلاً من تحميل المستودع من جديد."
    git pull origin main
else
    echo "تهيئة المستودع في المجلد الحالي..."
    git init  # تهيئة مستودع Git جديد
    git remote add origin "$REPO_URL"  # إضافة المستودع البعيد
    git fetch origin  # جلب البيانات من المستودع البعيد
    git reset --hard origin/main  # استعادة التحديثات من الفرع الرئيسي
fi

echo "تم تحميل المستودع أو تحديثه بنجاح!"
