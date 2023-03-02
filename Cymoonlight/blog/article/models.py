from django.db import models
from django.utils import timezone
# 限制权限
from django.contrib.auth.models import User
# markdown渲染
from markdown import Markdown

"""
修改后要进行数据迁移
(venv) > python manage.py makemigrations
(venv) > python manage.py migrate
"""
# 标题图
class Avatar(models.Model):
    content = models.ImageField(upload_to='avatar/%Y%m%d')

# 标签model
class Tag(models.Model):
    """标签"""
    text = models.CharField(max_length=30)

    class Meta:
        ordering = ['-id']

    def __str__(self):
        return self.text

# 文章分类model
class Category(models.Model):
    """文章分类"""
    title = models.CharField(max_length=100)
    created = models.DateTimeField(default=timezone.now)

    class Meta:
        ordering = ['-created']

    def __str__(self):
        return self.title




# 博客文章model
class Article(models.Model):
    # 标签
    tags = models.ManyToManyField(
        Tag,
        blank=True,
        related_name='articles'
    )

    # 分类
    category = models.ForeignKey(
        Category,
        null=True,
        blank=True,
        on_delete=models.SET_NULL,
        related_name='articles'
    )

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
    # 标题图
    avatar = models.ForeignKey(
        Avatar,
        null=True,
        blank=True,
        on_delete=models.SET_NULL,
        related_name='article'
    )

    def __str__(self):
        return self.title
    
    # 新增方法,将body转换为html标签的正文
    def get_md(self):
        md = Markdown(
            extensions=[
            'markdown.extensions.extra',
            'markdown.extensions.codehilite',
            'markdown.extensions.toc',
            ]
        )
        md_body = md.convert(self.body)
        # toc是渲染后的目录
        return md_body, md.toc
    
    # 规定查询排序
    class Meta:
        ordering = ['-created']
