# -*- coding: utf-8 -*-

from django.http import HttpResponse

from TestModel.models import Test

# 数据库添加操作
def dbaddTest(request):
    test1 = Test(name = "cy")
    test1.save()
    return HttpResponse("<h1>数据添加成功!</h1>")

# 数据库获取数据操作
def dbgetTest(request):
    # 初始化
    response = ""
    response1 = ""

    # 通过object这个模型管理器的all()获得所有数据行,相当于SQL中的SELECT * FROM
    list = Test.objects.all()

    # filter相当于SQL中的WHERE,可设置条件过滤结果
    response2 = Test.objects.filter(id = 1)

    # 获取单个对象(注意是对象)
    response3 = Test.objects.get(id = 1)

    # 限制返回的数据,相当于SQL中的 OFFSET 0 LIMIT 2
    response4 = Test.objects.order_by('name')[0:2]

    # 数据排序
    response5 = Test.objects.order_by("id")

    # 上面的方法可以连锁使用
    response6 = Test.objects.filter(name = "user").order_by('id')

    # 输出所有数据
    for var in list:
        response1 += var.name + " "
    response = response1
    return HttpResponse("<h1>" + response + "</h1>")

# 数据库更新数据操作
def dbupdateTest(request):
    # 修改其中一个id=1的name字段, 再save, 相当于SQL中的UPDATE
    test1 = Test.objects.get(id = 1)
    test1.name = 'cymoonlight'
    test1.save()

    # 另外一种方式
    # Test.objects.filter(id = 1).update(name = 'newcy')

    # 修改所有的列
    # Test.objects.all().update(name = 'cymoonlight')

    return HttpResponse("<h1>修改成功</h1>")

# 数据库删除数据操作
def dbdelTest(request):
    # 删除id = 1 的数据
    test1 = Test.objects.get(id = 1)
    test1.delete()

    # 另外一种方式
    # Test.objects.filter(id = 1).delete()

    # 删除所有数据
    # Test.objects.all().delete()

    return HttpResponse("<h1>删除完成</h1>")
