from django.db import models

# Create your models here.
class Test(models.Model):                   #类名 = 数据库表名,继承models.Model
    name = models.CharField(max_length=20)  #字段 = 数据表中的字段(name)   CharField = 数据类型varchar max_length = 参数限定长度