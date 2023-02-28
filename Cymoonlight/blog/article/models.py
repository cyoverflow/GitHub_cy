from django.db import models
from django.utils import timezone
# 限制权限
from django.contrib.auth.models import User

# 博客文章model
class Article(models.Model):
    # 文章作者
    author = models.ForeignKey(
        User,
        null=True,
        on_delete=models.CASCADE,
        related_name='articles'
    )
    # 标题
    title = models.CharField(max_length=100)
    # 正文
    body = models.TextField()
    # 创建时间
    created = models.DateTimeField(default=timezone.now)
    # 更新时间
    updated = models.DateTimeField(auto_now=True)

    def __str__(self):
        return self.title
