from django.contrib import admin
# 别忘了导入Article
from .models import Article

# 注册Article到admin中
admin.site.register(Article)