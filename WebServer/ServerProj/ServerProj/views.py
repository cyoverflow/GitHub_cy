from django.http import HttpResponse
from django.shortcuts import render, redirect


def homepage(request):
    return HttpResponse("<h1>Hello, homepage!</h1>")


def hello(request):
    context = {}
    context['hello'] = 'Hello World!'
    return render(request, 'test1.html', context)

# 变量


def varTest(request):
    views_name = "变量测试"
    return render(request, "variable_test.html", {"name": views_name})

# 列表


def listTest(request):
    views_list = ["列表元素1", "列表元素2", "列表元素3"]
    return render(request, "list_test.html", {"views_list": views_list})

# 字典


def dictTest(request):
    views_dict = {"name": "cy", "age": "24"}
    return render(request, "dict_test.html", {"views_dic": views_dict})

# 过滤器:default


def defTest(request):
    name = 0
    return render(request, "def_test.html", {"name": name})

# 过滤器:length
def lengthtest(request):
    views_list = ['red', 'green', 'blue', 'yellow', 'white', 'black']
    return render(request, "length_test.html", {"list" : views_list})

# 过滤器:filesizeformat
def fSizeFmttest(request):
    size = 2048
    return render(request, "fsizeFmt_test.html", {"size" : size})

# 过滤器:date
def datetest(request):
    import datetime
    time = datetime.datetime.now()
    return render(request, "date_test.html", {"time" : time})

# 过滤器:truncatechars
def truncatetest(request):
    name = "truncatechars_test"
    return render(request, "truncate_test.html", {"title" : name})

# 过滤器:safe
def safetest(request):
    views_str = "<a href='https://www.github.com/'>点击跳转到Github</a>"
    return render(request, "safe_test.html", {"views_str" : views_str})

# 过滤器:if/else标签
def ifelsetest(request):
    number = 98
    return render(request, "ifelse_test.html", {"grade" : number})

# 过滤器:for标签
def fortest(request):
    views_dict = {"name":"成员1", "age":"24", "地区":"广东"}
    return render(request, "for_test.html", {"dict" : views_dict})

# 静态文件导入图片
def imgtest(request):
    name ="图片显示"
    return render(request, "static_test.html", {"name": name})

# 模板继承测试
def extendtest(request):
    return render(request, "extend_test.html")

# GET方法测试
def gettest(request):
    name = request.GET.get('name')
    return HttpResponse('<h1>姓名:{}</h1>'.format(name))

# POST方法测试
def posttest(request):
    name = request.POST.get('name')
    return HttpResponse('姓名:{}'.format(name))

# body测试
def bodytest(request):
    name = request.body
    print(name)
    return HttpResponse("body test")

# path测试
def pathtest(request):
    name = request.path
    print(name)
    return HttpResponse("path test")

# method测试
def methodtest(request):
    name = request.method
    print(name)
    return HttpResponse("method test")

# HttpResponse响应测试
def httpresTest(request):
    #return HttpResponse("Http Response Test")
    return HttpResponse("<a href='https://www.github.com/'>Github</a>")

# render响应测试
def renderTest(request):
    name = "Render Test"
    return render(request, "xxxx.html", {"name" : name})

# redirect响应测试
def redirectTest(request):
    return redirect("/test1/")