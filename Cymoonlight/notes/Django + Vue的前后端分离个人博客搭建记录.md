# Django + Vue的前后端分离个人博客搭建记录

## 1. 前后端分离

**传统的开发协作模式**：

- 前端写好静态的HTML页面交付给后端开发。
- 后端使用模板引擎去套模板，同时内嵌模板变量和逻辑。
- 前后端集成对接，遇到问题前台后台都返工。
- 集成调试，直至成功。

这种模式的**问题**在于：

- 前端调试要安装完整的后端开发工具，并且遇到问题需要后端开发帮忙调试。
- 前后端耦合，甚至还要求后端人员会 Javascript 等前端语言。
- 前端页面里嵌入后端的代码，一旦后端换了语言，简直就要重做。

对于大项目来说，这样的模式增加了沟通成本，前后端的开发进度相互影响，从而降低了开发效率。

而在**前后端分离模式**下，前端和后端只通过简单的接口（API）进行数据交流，而不用像模板将数据藏在某个 div 里。前后端工程师约定好数据接口，并行开发互不影响。即使其中一方的需求发生变更，只要接口不变，对方就不需要更改代码。

前后端分离模式**摆脱了笨重的模板引擎**，是当前 web 开发的大趋势之一。

## 2.开发环境

- 服务器：**CentOS 7.6**
- 后端：**Django 3.1.3**
- 前端：**Vue 3**
- 开发系统：**Win 10**

## 3.搭建开发环境

### 配置虚拟环境

**虚拟环境**是 Python 多版本管理的利器，可以使每个项目环境与其他项目独立开来，保持环境的干净，解决包冲突问题。你可以将虚拟环境理解为一个隔绝的小系统。

**从 Python 3.3 版本开始就自带了虚拟环境安装指令，配置一下就可以用了。**

在命令行中新建目录 `/cymoonlight`。进入此目录：

```
D:\>cd cymoonlight
D:\cymoonlight>
```

输入配置 venv 的命令，其中的`venv`为虚拟环境的目录：

```
D:\cymoonlight> python -m venv venv  
```

创建完成后，输入`venv\Scripts\activate.bat`，即可进入虚拟环境：

```
D:\cymoonlight> venv\Scripts\activate.bat
(venv) D:\cymoonlight>
```

**盘符前有`(venv)`标识说明进入venv成功。**

> 若上述方法不成功，则可以通过 `virtualenv` 库创建虚拟环境。

### 安装Django

**在虚拟环境下**，输入命令 `pip install django==3.1.3`：

```
(venv) D:\cymoonlight> pip install django==3.1.3

Collecting django==3.1.3
  Using cached 
  ...
  ...
Successfully installed django-3.1.3

(venv) D:\cymoonlight>
```

系统打印出以上文字表示 Django 安装成功了。（提示符以 `(venv)` 开头）

> 由于国内复杂的网络环境， Pip 的下载可能非常缓慢甚至失败。国内用户请更换国内的镜像下载源。

### 创建Django项目

还是在**虚拟环境**下，在`cymoonlight`文件夹中创建一个叫 `blog` 的Django项目：

```
(venv) D:\cymoonlight> django-admin startproject blog
```

查看`cymoonlight`文件夹，发现多了`blog`文件夹，其结构应该是这样：

```
blog
│  manage.py
│
└─blog
        asgi.py
        settings.py
        urls.py
        wsgi.py
        __init__.py
```

### 运行Django服务器

Django 自带一个轻量的 Web 开发服务器，被叫做 runserver。

开发服务器是为了让你快速开发Web程序，通过它可以避开配置生产环境的服务器的繁琐环节。

开发服务器会自动的检测代码的改变，并且自动加载它，因此在修改代码后不需要手动去重启服务器，非常的方便。

要运行这个服务器，首先要进入`blog`文件夹，即含有`manage.py`文件的那个：

```
(venv) D:\cymoonlight> cd blog
(venv) D:\cymoonligh\blog>
```

输入命令`python manage.py runserver`：

```
(venv) D:\cymoonligh\blog> python manage.py runserver
Performing system checks...
...
Django version 3.1.3, using settings 'blog.settings'
Starting development server at http://127.0.0.1:8000/
Quit the server with CTRL-BREAK.
```

系统打印出这些信息，说明服务器启动成功了。

打开 Chrome 浏览器，输入http://127.0.0.1:8000/ ，网页中看到一个绿色的小火箭，恭喜你，项目已经正常运行了。

## 4.初识Django REST framework

### DRF开发准备

首先在命令行创建博客文章的 App：

```
(venv) > python manage.py startapp article
```

创建一个简单的博客文章模型：

```
# article/models.py

from django.db import models
from django.utils import timezone

# 博客文章model
class Article(models.Model):
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

```

前后端分离中涉及到一个重要的概念：**序列化**（后面讲解）。Django 有一个非常优秀的库 Django Rest Framework（后称 **DRF**）。它可以帮我们封装好序列化的底层实现，让开发者专注于业务本身。

安装 DRF 及其他依赖库：

```
pip install djangorestframework==3.12.2
pip install markdown==3.3.3
pip install django-filter==2.4.0
```

然后将 App 注册列表：

```python
# blog/settings.py

INSTALLED_APPS = [
    ...

    'rest_framework',
    'article',
]
```

接着还需要添加 DRF 的登录视图，以便 DRF 自动为你的可视化接口页面生成一个用户登录的入口：

> 后续开发出接口页面后试着把这行代码删掉，看看会有什么不同。

```
# blog/urls.py

...
from django.urls import include

urlpatterns = [
    ...
    path('api-auth/', include('rest_framework.urls')),
]
```

最后记得数据迁移：

```
(venv) > python manage.py makemigrations
(venv) > python manage.py migrate
```

准备工作就做好了。

### 序列化与Django

前后端分离的核心思想之一，就是两端交互不通过模板语言，而只传输需要的数据。因此问题就来了。

在 Django 程序的运行过程中，变量都是存储在服务器的内存中；更要命的是，后端 Django 程序中存储的是 Python 变量，而前端的浏览器中是 Javascript 变量，这两者是无法直接通过你家的网线进行传递和交流的。因此需要规定一个“标准格式”，前后端都根据这个标准格式，对资源进行保存、读取、传输等操作。

`JSON` 就是这种标准格式之一。它很轻量，表示出来就是个字符串，可以直接被几乎所有的语言读取，非常方便。

举个例子，把 Python 对象转换为 JSON ，这被称为**序列化**（serialization）：

```
>>> import json
>>> person = dict(name='Trump', age=82)
>>> json.dumps(person)
# 这是个字符串
'{"age": 82, "name": "Trump"}'
```

把 JSON 转换为 Javascript 对象，被称为**反序列化**：

```
>>> json_str = '{"age": 82, "name": "Trump"}'
>>> json.loads(json_str)
# 这是个 js 对象
{'age': 82, 'name': 'Trump'}
```

总之，把变量从内存中变成可存储或传输的过程称之为**序列化**，反过来把变量内容从序列化的对象重新读到内存里称之为**反序列化**。

回顾 **Django 传统流程对一个网络请求的处理**：

```
def a_list(request):
    articles = Article.objects.all()
    return render(..., context={'articles': articles})
```

视图函数将数据作为上下文返回，通过模板引擎将上下文渲染为页面中的数据。

**Restful** 的处理流程仅增加了一步，即对数据**序列化**的处理：

```
def a_list(request):
    articles = Article.objects.all()
    # 序列化数据
    serializer = Serializer(articles, many=True)
    return JsonResponse(serializer.data, safe=False)
```

数据被**序列化为 Json 字符串，直接交由前端**处理。

这就是前后端分离的雏形：

- 后端提供数据；
- 前端专注于操作数据、渲染页面。

### Hello World

按照这个思路，我们来写一个文章列表接口吧。

`article` 模型在前面已经写好了，接下来写视图：

```
# article/views.py

from django.http import JsonResponse
from article.models import Article
from article.serializers import ArticleListSerializer

def article_list(request):
    articles = Article.objects.all()
    serializer = ArticleListSerializer(articles, many=True)
    return JsonResponse(serializer.data, safe=False)
```

代码一共就 3 行：

- 取出所有文章的 `QuerySet`；
- 根据 `QuerySet` 数据，创建一个序列化器；
- 将序列化后的数据以 Json 的形式返回。

跟说好的一样，返回的东西不再是传统的模板了，而是 Json 数据。

代码里的序列化器 `ArticleListSerializer` 我们还没写，接下来就完成它。

新建一个 `article/serializers.py` 的文件，写入下面的代码：

```
# article/serializers.py

from rest_framework import serializers

class ArticleListSerializer(serializers.Serializer):
    id = serializers.IntegerField(read_only=True)
    title = serializers.CharField(allow_blank=True, max_length=100)
    body = serializers.CharField(allow_blank=True)
    created = serializers.DateTimeField()
    updated = serializers.DateTimeField()
```

序列化类看起来与 Django 的 `Form` 表单类非常的类似。它指定了接口数据中各个字段的具体类型，自动对**请求**和**响应**中的数据进行序列化和反序列化的转换。其底层实现逻辑已经由 DRF 框架封装好了，在入门阶段通常不需要你操心。

最后将各级 `urls.py` 配置好：

```python
# blog/urls.py
...
urlpatterns = [
    ...
    path('api/article/', include('article.urls', namespace='article')),
]
```

以及：

```python
# article/urls.py

from django.urls import path
from article import views

app_name = 'article'

urlpatterns = [
    path('', views.article_list, name='list'),
]
```

代码部分就完成了。

### 创建管理员账号(Superuser)

管理员账号（Superuser）是可以进入网站后台，对数据进行维护的账号，具有很高的权限。这里我们需要创建一个管理员账号，以便添加后续的测试数据。

**虚拟环境**中输入`python manage.py createsuperuser`指令，创建管理员账号：

```
(env) D:\cymoonlight\blog>python manage.py createsuperuser
Username: cy99
Email address: ckddn0925@163.com
Password:
Password (again):
Superuser created successfully.
```

指令会提示你输入账号名字、邮箱和密码，根据喜好填入即可。

**将Article注册到后台中:**

接下来我们需要“告诉”Django，后台中需要添加`Article`这个数据表供管理。

打开`article/admin.py`，写入以下代码：

```
article/admin.py

from django.contrib import admin

# 别忘了导入Article
from .models import Article

# 注册ArticlePost到admin中
admin.site.register(Article)
```

这样就简单的注册好了。

**在后台中测试**

Django项目生成的时候就自动配置好了后台的settings和url，因此不需要我们再操心了。

启动server，在浏览器中输入`http://127.0.0.1:8000/admin/`，一切正常的话就看到下面的**登录界面**了：

![image-20230227222038261](assets/image-20230227222038261.png)

输入刚才创建的管理员账号，登录进去：

![image-20230227222221885](assets/image-20230227222221885.png)

红框内就是刚才添加的`Article`数据表，点击进入后，再点击右上角的`ADD ARTICLE`按钮，到达如下页面：

![image-20230227222315009](assets/image-20230227222315009.png)

仔细看一下表单中的每一项，发现与`Article`中的字段完全符合；因为`updated`字段指定了自动添加，这里就没显示了。

将表单填好后，点击保存：

![image-20230227222504102](assets/image-20230227222504102.png)

接下来在后台中随意给 `article` 添加几个测试数据，启动服务器并在浏览器中访问 `http://127.0.0.1:8000/api/article/`，可以看到页面中返回的 Json 字符串如下（稍作了排版）：

```json
[
    {
        "id": 1, 
        "title": "First Article", 
        "body": "Hello everyone, hello world!", 
        "created": "2023-02-27T14:23:03Z", 
        "updated": "2023-02-27T14:24:54.630532Z"
    }, 
    {
        "id": 2, 
        "title": "Article no.2", 
        "body": "this is the second article here", 
        "created": "2023-02-27T14:25:45Z", 
        "updated": "2023-02-27T14:26:13.508621Z"
    }, 
    {
        "id": 3, 
        "title": "Third one", 
        "body": "hello!", 
        "created": "2023-02-27T14:26:42Z", 
        "updated": "2023-02-27T14:26:52.002129Z"
    }
]
```

目前为止已经成功完成了一个简单的接口。

## 5.序列化器与视图函数

### Model Serializer

上一章我们借助 DRF 库，写了一个简单的序列化器：

```
class ArticleListSerializer(serializers.Serializer):
    id = serializers.IntegerField(read_only=True)
    title = serializers.CharField(allow_blank=True, max_length=100)
    body = serializers.CharField(allow_blank=True)
    created = serializers.DateTimeField()
    updated = serializers.DateTimeField()
```

这个序列化器长得跟 `Form` 或者 `Model` 也太像了，甚至感觉代码都重复了。如果能再简化一下就更好了。DRF 也考虑到了这一点，提供了现成的解决方案，也就是 `ModelSerializer` 了。

将序列化器修改为下面这样：

```
# article/serializers.py

from rest_framework import serializers
from article.models import Article

# 父类变成了 ModelSerializer
class ArticleListSerializer(serializers.ModelSerializer):
    class Meta:
        model = Article
        fields = [
            'id',
            'title',
            'created',
        ]
```

`ModelSerializer` 的功能与上一章的 `Serializer` 基本一致，不同的是它额外做了些工作：

- 自动推断需要序列化的字段及类型
- 提供对字段数据的验证器的默认实现
- 提供了修改数据需要用到的 `.create()` 、 `.update()` 方法的默认实现

另外我们还可以在 `fileds` 列表里挑选出需要的数据，以便减小数据的体积。

在浏览器中访问 `http://127.0.0.1:8000/api/article/`，页面中呈现的数据如下（略微排版后）：

```json
[
    {
        "id": 1, 
        "title": "First Article", 
        "created": "2023-02-27T14:23:03Z"
    }, 
    {
        "id": 2, 
        "title": "Article no.2",
        "created": "2023-02-27T14:25:45Z"
    }, 
    {
        "id": 3, 
        "title": "Third one", 
        "created": "2023-02-27T14:26:42Z"
    }
]
```

可以看到 Json 数据仅包含 `fields` 中规定的字段了。

### API View

除了对序列化器的支持以外，DRF 还提供了**对视图的扩展**，以便视图更好的为接口服务。

将文章的视图修改为如下：

```python
# article/views.py

from rest_framework.decorators import api_view
from rest_framework.response import Response
from rest_framework import status
from article.models import Article
from article.serializers import ArticleListSerializer

@api_view(['GET', 'POST'])
def article_list(request):
    if request.method == 'GET':
        articles = Article.objects.all()
        serializer = ArticleListSerializer(articles, many=True)
        return Response(serializer.data)
    
    elif request.method == 'POST':
        serializer = ArticleListSerializer(data=request.data)
        if serializer.is_valid():
            serializer.save()
            return Response(serializer.data, status=status.HTTP_201_CREATED)
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
```

主要的变化如下：

- `@api_view` 装饰器允许视图接收 `GET` 、`POST` 请求，以及提供如 `405 Method Not Allowed` 等默认实现，以便在不同的请求下进行正确的响应。
- 返回了 `Response` ，该对象由 Django 原生响应体扩展而来，它可以根据**内容协商**来确定返回给客户端的正确内容类型。如果数据验证有误，还可以返回适当的状态码以表示当前的情况。

让我们刷新文章列表接口：

![image-20230228145710399](assets/image-20230228145710399.png)

出现了可视化的接口界面！

这就是视图中 `Response` 提供的**内容协商**能力了。也就是说，Django 后端根据客户端请求响应的内容类型不同，自动选择适合的表现形式；浏览器请求资源时，就返回可视化的 HTML 资源表示，其他形式请求时，又可以返回 Json 纯数据的形式。这给开发带来极大的方便。

有同学就问了，那我怎么验证它是不是真的会返回 Json 数据呢？让我们发一个网络请求试试看。

### httpie

在命令行中发送请求，笔者推荐 [httpie](https://github.com/jakubroztocil/httpie#installation) 这个库，基于 Python 并且小巧美观。

重新打开一个命令行，安装方式就是万能的 `pip`：

> httpie 可以安装在全局。

```
pip install httpie
```

安装成功后，试试在命令行提交一个请求：

```
C:\...> http http://127.0.0.1:8000/api/article/

# 以下为返回内容
HTTP/1.1 200 OK
Content-Length: 235
Content-Type: application/json
...

[
    {
        "created": "2020-06-15T09:24:18Z",
        "id": 1,
        "title": "My first post"
    },
    {
        "created": "2020-06-15T09:24:38Z",
        "id": 2,
        "title": "Another post"
    },
    {
        "created": "2020-06-15T09:24:58Z",
        "id": 3,
        "title": "Third article with awesome things"
    }
]
```

再试试新建文章：

```
C:\...> http POST http://127.0.0.1:8000/api/article/ title=PostByJson body=HelloWorld!

# 以下为返回内容
HTTP/1.1 201 Created
...
{
    "created": "2020-06-29T07:47:53.984916Z",
    "id": 4,
    "title": "PostByJson"
}
```

浏览文章列表、新建文章接口就完成了。

### 用Postman代替httpie进行web请求测试

使用 httpie 需要一定的学习成本，并且在 Windows 命令行下有符号兼容的坑，所以使用Postman来替代httpie

直接进入官网并下载安装即可

 **使用方法：**![image-20230228160157503](assets/image-20230228160157503.png)

**发送GET请求**

* 在URL框中输入地址
* 点击Send即可

**发送POST请求**

* 在URL框中输入地址
* 在Body中选择raw-JSON
* 写入请求体内容并点击Send

![image-20230228161837142](assets/image-20230228161837142.png)

## 6.基于类的视图

### 文章详情接口

传统 Django 中就有**基于类的视图**的存在，DRF 中自然也有。优点也都差不多，即实现功能的模块化继承、封装，减少重复代码。接下来就用**文章详情**接口练练手。

首先在视图中新增下面的代码：

```python
# article/views.py

from rest_framework.response import Response
from rest_framework.views import APIView
from rest_framework import status

from django.http import Http404

from article.models import Article
from article.serializers import ArticleDetailSerializer

class ArticleDetail(APIView):
    """文章详情视图"""

    def get_object(self, pk):
        """获取单个文章对象"""
        try:
            # pk即主键(primary key),默认状态下是id
            return Article.objects.get(pk=pk)
        except:
            raise Http404
        
    def get(self, request, pk):
        article = self.get_object(pk)
        serializer = ArticleDetailSerializer(article)
        # 返回Json数据
        return Response(serializer.data)
    
    def put(self, request, pk):
        article = self.get_object(pk)
        serializer = ArticleDetailSerializer(article, data=request.data)
        # 验证提交的数据是否合法
        # 不合法则返回404
        if serializer.is_valid():
            # 序列化器将持有的数据反序列化后,
            # 保存到数据库中
            serializer.save()
            return Response(serializer.data)
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
    
    def delete(self, request, pk):
        article = self.get_object(pk)
        article.delete()
        # 删除成功后返回204
        return Response(status=status.HTTP_204_NO_CONTENT)
```

代码不复杂，就是提供了对文章详情的**获取**、**修改**、**删除**的 3 个方法，以及 1 个用于获取单个文章 model 的辅助方法。和之前说的一样，DRF 类视图与传统 Django 的区别，`.get()` 、 `.put()` 就是多了一个将对象序列化（或反序列化）的步骤。`.delete()` 方法因为不用返回实际数据，执行完删除动作就OK了。

> 从这个地方就可以看出，序列化器 `serializer` 不仅可以将数据进行序列化、反序列化，还包含数据验证、错误处理、数据库操作等能力。
>
> 序列化这个概念与具体语言无关。Python 或 JavaScript 对象转换为 Json 都称为序列化，反之为反序列化。Json 是两种语言传输信息的桥梁，一但信息到达，对方都需要将其还原为自身的数据结构。

由于详情接口需要返回完整的字段数据（与阉割版的文章列表接口不同），所以要重新给它定义一个序列化器：

```python
# article/serializers.py

...

class ArticleDetailSerializer(serializers.ModelSerializer):
    class Meta:
        model = Article
        fields = '__all__'
```

`fields = '__all__'` 代表要使用所有字段。

配置 `urls.py`：

```
# article/urls.py

...

urlpatterns = [
    ...
    path('<int:pk>/', views.ArticleDetail.as_view(), name='detail'),
]
```

利用Postman发送一个**获取文章详情**请求试试：

![image-20230228164529121](assets/image-20230228164529121.png)

再试试**修改文章**接口：（注意请求方式变成了 `PUT`）

![image-20230228165122848](assets/image-20230228165122848.png)

再试试**删除文章**

![image-20230228170048981](assets/image-20230228170048981.png)

这篇 `id=4` 的文章就被删除掉了。

### 通用视图

对数据的增删改查是几乎每个项目的通用操作，因此可以通过 DRF 提供的 Mixin 类直接集成对应的功能。

修改一下 `ArticleDetail` 视图：

```
# article/views.py

...
from rest_framework import mixins
from rest_framework import generics

class ArticleDetail(mixins.RetrieveModelMixin,
                    mixins.UpdateModelMixin,
                    mixins.DestroyModelMixin,
                    generics.GenericAPIView):
    """文章详情视图"""
    queryset = Article.objects.all()
    serializer_class = ArticleDetailSerializer

    def get(self, request, *args, **kwargs):
        return self.retrieve(request, *args, **kwargs)

    def put(self, request, *args, **kwargs):
        return self.update(request, *args, **kwargs)

    def delete(self, request, *args, **kwargs):
        return self.destroy(request, *args, **kwargs)
```

使用 Mixin 已经足够简单了，但我们还可以让它更简单：

```
# article/views.py

...
class ArticleDetail(generics.RetrieveUpdateDestroyAPIView):
    queryset = Article.objects.all()
    serializer_class = ArticleDetailSerializer
```

发送请求试试，功能和最开头那个继承 `APIView` 的视图是完全相同的。

列表接口也可以做同样的修改：

```python
# article/views.py

...
class ArticleList(generics.ListCreateAPIView):
    queryset = Article.objects.all()
    serializer_class = ArticleListSerializer
```

对urls.py也进行修改：

```python
# article/urls.py

from django.urls import path
from article import views

app_name = 'article'

urlpatterns = [
    path('', views.ArticleList.as_view(), name='list'),
    ...
]
```

> 除了上述介绍的以外，框架还提供 `ListModelMixin`、`CreateModelMixin` 等混入类或通用视图，覆盖了基础的增删改查需求。

## 7.限制用户权限

权限是 web 应用的重要组成部分。没有权限控制，任何人都可以对任何资源进行更改操作，那就太恐怖了。

本章来看看 DRF 中如何进行权限管理。

### 文章与用户

依靠用户身份来限制权限是比较通用的做法。这就需要给文章模型添加用户外键，确定每篇文章的作者了。保险起见，首先删除现有的所有文章数据。

修改文章的 model，让每篇文章都对应一个作者：

```python
# article/models.py

...

from django.contrib.auth.models import User

class Article(models.Model):
    author = models.ForeignKey(
        User, 
        null=True,
        on_delete=models.CASCADE, 
        related_name='articles'
    )
    ...
```

执行迁移：

```
> python manage.py makemigrations
> python manage.py migrate
```

启动服务器后，查看当前文章列表：

![image-20230228211905228](assets/image-20230228211905228.png)

空空如也。

接下来就拿文章列表接口开刀。

### 权限控制

DRF 内置了如 `IsAuthenticated`、`IsAdminUser`、`AllowAny` 等权限控制类。

由于是个人博客，因此只准许管理员发布文章。修改文章列表视图如下：

```
# article/views.py

...
from rest_framework.permissions import IsAdminUser

class ArticleList(generics.ListCreateAPIView):
    ...
    # 新增
    permission_classes = [IsAdminUser]
```

`permission_classes` 可以接收一个列表，因此权限控制类可以设置多个，请求必须满足所有控制条件才允许被放行。

测试一下：

<img src="assets/image-20230228212305353.png" alt="image-20230228212305353"  />

![image-20230228212348760](assets/image-20230228212348760.png)

倒是确实起作用了，但是除了管理员之外其他人**连查看都没权限了**，显然这不是我们想要的。

好在**自定义**一个权限类也不难。在文章 app 中创建 `article/permissions.py` 文件，写入以下代码：

```python
# article/permissions.py

from rest_framework import permissions

class IsAdminUserOrReadOnly(permissions.BasePermission):
    """
    仅管理员用户可进行修改
    其他用户仅可查看
    """
    def has_permission(self, request, view):
        # 对所有人允许 GET, HEAD, OPTIONS 请求
        if request.method in permissions.SAFE_METHODS:
            return True

        # 仅管理员可进行其他操作
        return request.user.is_superuser
```

自定义的权限类继承了 `BasePermission` 这个基础的父类，并实现了父类中的钩子方法 `def has_permission`。此方法在每次请求到来时被唤醒执行，里面简单判断了请求的种类是否安全（即不更改数据的请求），如果安全则直接通过，不安全则只允许管理员用户通过。

再次修改视图：

```
# article/views.py

...

# from rest_framework.permissions import IsAdminUser
from article.permissions import IsAdminUserOrReadOnly

class ArticleList(generics.ListCreateAPIView):
    ...
    permission_classes = [IsAdminUserOrReadOnly]

# 顺便把详情视图的权限也更改了
class ArticleDetail(generics.RetrieveUpdateDestroyAPIView):
    ...
    permission_classes = [IsAdminUserOrReadOnly]
```

这就可以了。

首先测试一下**用户未登录**时的情况：

![image-20230228213149892](assets/image-20230228213149892.png)

![image-20230228213224773](assets/image-20230228213224773.png)

在后台中创建一个普通用户 **sleepyjoe**

```
> python manage.py shell
Python 3.6.8 (tags/v3.6.8:3c6b436a57, Dec 24 2018, 00:16:47) [MSC v.1916 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
(InteractiveConsole)
>>> from django.contrib.auth.models import User
>>> user = User.objects.create_user('sleepyjoe', 'adccc@qq.com', '1234')
>>> user.save()
```

### Postman进行身份认证

* 出于安全考虑，使用变量来存储账号密码

   ![image-20230228221032263](assets/image-20230228221032263.png)

* 在请求页面的**Authorization - Type - Basic Auth**输入用户名和密码

   ![image-20230228221323666](assets/image-20230228221323666.png)

用普通用户身份进行请求：

GET： ![image-20230228222201351](assets/image-20230228222201351.png)

POST： ![image-20230228222227754](assets/image-20230228222227754.png)

用管理员身份进行请求：

GET： ![image-20230228222427814](assets/image-20230228222427814.png)

POST：![image-20230228222517176](assets/image-20230228222517176.png)

任何人都可以查看资源；但是新增（CREATE）、更新（PUT）、删除（DELETE）等修改操作就只允许管理员执行。

## 8.文章关联用户

### 将字段设置为只读

上一章中我们已经将用户以外键的形式关联到文章中了，但是由于 `author` 字段是允许为空的，所以理论上还是可以发表没有作者的文章：

 ![image-20230302153038838](assets/image-20230302153038838.png)

虽然你可以直接指定作者的 `id` 值来对外键赋值，但是这种方法不但没有必要，甚至还可以伪造一个错误的用户 `id` ：

 ![image-20230302153246468](assets/image-20230302153246468.png)

解决方法如下：既然请求体中已经包含用户信息了，那就可以从 `Request` 中提取用户信息，并把额外的用户信息注入到已有的数据中。

修改视图：

```python
# article/views.py

class ArticleList(generics.ListCreateAPIView):
    ...
    # 新增代码
    def perform_create(self, serializer):
        serializer.save(author=self.request.user)
```

- 新增的这个 `perform_create()` 从父类 `ListCreateAPIView` 继承而来，它在序列化数据真正保存之前调用，因此可以在这里添加额外的数据（即用户对象）。
- `serializer` 参数是 `ArticleListSerializer` 序列化器实例，并且已经携带着验证后的数据。它的 `save()` 方法可以接收关键字参数作为额外的需要保存的数据。

在命令行重新测试：

 ![image-20230302154050337](assets/image-20230302154050337.png)

很好，但是用户依然可以手动传入一个错误的 `author`：

 ![image-20230302154529003](assets/image-20230302154529003.png)

好在序列化器允许你指定只读字段。修改 `ArticleListSerializer`：

```python
# article/serializers.py

...

class ArticleListSerializer(serializers.ModelSerializer):
    class Meta:
        ...
        # 新增代码
        read_only_fields = ['author']
```

此时在接收 POST 请求时，序列化器就不再理会请求中附带的 `author` 数据了：

 ![image-20230302154807433](assets/image-20230302154807433.png)

### 显示用户信息

虽然作者外键已经出现在序列化数据中了，但是仅仅显示作者的 id 不太有用，我们更想要的是比如名字、性别等更具体的结构化信息。所以就需要将序列化数据**嵌套**起来。

新创建一个用户 app：

```
(venv) [worksplace] D:\develop\git_repo\GitHub_cy\Cymoonlight\blog
> manage.py startapp user_info
```

并将新 app 添加到注册列表：

```python
# blog/settings.py

INSTALLED_APPS = [
    ...
    'user_info',
]
```

新建 `user_info/serializers.py` 文件，写入：

```python
# user_info/serializers.py

from django.contrib.auth.models import User
from rest_framework import serializers

class UserDescSerializer(serializers.ModelSerializer):
    """于文章列表中引用的嵌套序列化器"""

    class Meta:
        model = User
        fields = [
            'id',
            'username',
            'last_login',
            'date_joined'
        ]
```

序列化类我们已经比较熟悉了，这个序列化器专门用在文章列表中，展示用户的基本信息。

最后修改文章列表的序列化器，把它们嵌套到一起：

```python
# article/serializers.py

from user_info.serializers import UserDescSerializer

class ArticleListSerializer(serializers.ModelSerializer):
    # read_only 参数设置为只读
    author = UserDescSerializer(read_only=True)

    class Meta:
        model = Article
        fields = [
            'id',
            'title',
            'created',
            'author',
        ]
        # 嵌套序列化器已经设置了只读，所以这个就不要了
        # read_only_fields = ['author']
```

这就 OK 了，在命令行测试一下：

 ![image-20230302155855167](assets/image-20230302155855167.png)

## 9.超链接与分页

### 增加url字段

目前我们的文章列表是这样的：

```
...
{
        "id": 7,
        "title": "post with user 2",
        "created": "2023-03-02T07:39:58.123365Z",
        "author": {
            "id": 1,
            "username": "cy99",
            "last_login": "2023-02-27T14:21:15.057700Z",
            "date_joined": "2023-02-27T14:12:49.451904Z"
        }
    },
    {
        "id": 8,
        "title": "post for readonly author",
        "created": "2023-03-02T07:47:50.480534Z",
        "author": {
            "id": 1,
            "username": "cy99",
            "last_login": "2023-02-27T14:21:15.057700Z",
            "date_joined": "2023-02-27T14:12:49.451904Z"
        }
    }
]
```

通过数据看不出每篇文章的实际 url 地址。虽然包含了文章的 id 号，但 id 和 url 不一定总是关联的。最好 json 数据中直接提供**超链接**到每篇文章的 url，以后前端用起来就方便了。

实现超链接可以用 DRF 框架提供的 `HyperlinkedIdentityField` ：

```python
# article/serializers.py

...

class ArticleListSerializer(serializers.ModelSerializer):
    # 新增字段，添加超链接
    url = serializers.HyperlinkedIdentityField(view_name="article:detail")
    ...

    class Meta:
        model = Article
        fields = [
            # 有了 url 之后，id 就不需要了
            'url',
            # 'id',
            ...
        ]
```

- `HyperlinkedIdentityField` 是 DRF 框架提供的超链接字段，只需要你在参数里提供路由的名称，它就自动帮你完成动态地址的映射。
- `view_name` 是路由的名称，也就是我们在 `path(... name='xxx')` 里的那个 name
- 别忘了在序列化器的 `fields` 列表里加上 `url`

在命令行中重新发个请求：

 ![image-20230302161342260](assets/image-20230302161342260.png)

这样就人性化多了。

> DRF 框架还提供了一个专门的超链接序列化器 `HyperlinkedModelSerializer`，大体上跟普通序列化器差不多，不同的是默认以超链接来表示关系字段。详情见[官方文档](https://www.django-rest-framework.org/api-guide/serializers/#hyperlinkedmodelserializer)。

### 分页

DRF 框架继承了 Django 方便易用的传统，分页这种常见功能提供了默认实现。

你只需要在 `settings.py` 里配置一下就行了：

```
# blog/settings.py

REST_FRAMEWORK = {
    'DEFAULT_PAGINATION_CLASS': 'rest_framework.pagination.PageNumberPagination',
    'PAGE_SIZE': 2
}
```

为了测试把每页的文章数量设置到 2。

在命令行发送请求：

 ![image-20230302161943391](assets/image-20230302161943391.png)

DRF 非常聪明的封装了分页相关的元信息：

- count：总页数
- next：下一页的 url
- previous：上一页的 url

实际的数据被放到 `results` 列表中。

试着获取第二页的数据：

 ![image-20230302162206665](assets/image-20230302162206665.png)

超链接和分页就完成了。代码量非常少，原因就是 DRF 把常用功能都内置了，直接调用就行，很适合快速开发。

## 10.视图集

通过前几章的折腾，我们已经把文章增删改查都完成了。经过合理运用类和继承的理念，代码已经相当精简了。但是， DRF 框架提供了更高层的抽象，可以让代码量进一步的减少。

来看看**视图集**的威力吧。

### 重构代码

大部分对接口的操作，都是在增删改查的基础上衍生出来的。既然这样，**视图集**就将这些通用操作集成在一起了。

试下用视图集重构代码。

首先将之前写的与文章有关的**序列化器**都注释掉，新增一个提供给视图集的新序列化器：

```python
# article/serializers.py

...

class ArticleSerializer(serializers.HyperlinkedModelSerializer):
    author = UserDescSerializer(read_only=True)

    class Meta:
        model = Article
        fields = '__all__'

...
```

序列化器继承的 `HyperlinkedModelSerializer` 基本上与之前用的 `ModelSerializer` 差不多，区别是它自动提供了外键字段的超链接，并且默认不包含模型对象的 id 字段。

接着把之前写的文章**视图**也全注释掉，并新增代码：

```python
# article/views.py

...
from article.models import Article
from article.permissions import IsAdminUserOrReadOnly
from rest_framework import viewsets
from article.serializers import ArticleSerializer

class ArticleViewSet(viewsets.ModelViewSet):
    queryset = Article.objects.all()
    serializer_class = ArticleSerializer
    permission_classes = [IsAdminUserOrReadOnly]

    def perform_create(self, serializer):
        serializer.save(author=self.request.user)
```

视图集类把前面章节写的列表、详情等逻辑都集成到一起，并且提供了默认的增删改查的实现。

> 这些基础逻辑都在父类 `ModelViewSet` 里封装实现了，有兴趣可耐心研究源码。

`perform_create()` 跟之前一样，在创建文章前，提供了视图集无法自行推断的用户外键字段。

由于使用了视图集，我们甚至连**路由**都不用自己设计了，使用框架提供的 `Router` 类就可以自动处理视图和 url 的连接。

修改**项目根路由**：

```python
# blog/urls.py

...

from rest_framework.routers import DefaultRouter
from article import views

router = DefaultRouter()
router.register(r'article', views.ArticleViewSet)

urlpatterns = [
    ...
    
    # drf 自动注册路由
    path('api/', include(router.urls)),

    # article/urls.py 可以全注释掉，不需要了
    # path('api/article/', include('article.urls', namespace='article')),

]
```

最后为了让分页更准确，给模型类规定好查询排序：

```python
# article/models.py

...

# 已经有的博客文章 model
class Article(models.Model):
    ...

    class Meta:
        ordering = ['-created']
```

完成了。

发送命令，进行测试：

 ![image-20230302164228719](assets/image-20230302164228719.png)

`Router` 类送给我们一个接口导航！

顺着导航里给的链接再试试：

 ![image-20230302164328445](assets/image-20230302164328445.png)

正确的显示了列表。再顺着列表提供的详情页点进去也肯定是没问题的。权限控制也与之前的完全一样。

就这么几行代码，就完成了一整套的接口操作！

**视图集**最大程度地减少需要编写的代码量，并允许你专注于 API 提供的交互和表示形式，而不是 URL 的细节。但并不意味着用它总是比构建单独的视图更好。

> 原因就是它的抽象程度太高了。如果你对 DRF 框架的理解不深并且需要做某种定制化业务，可能让你一时间无从下手。

在**精简**和**可读**之间，你应该根据实际情况进行取舍

### 覆写序列化器

前几章用普通视图分别实现了列表和详情接口，并且不同的接口对应了不同的序列化器。

虽然视图集默认只提供一个序列化器，但是通过覆写 `get_serializer_class()` 方法可以根据条件而访问不同的序列化器：

```python
class ArticleViewSet(viewsets.ModelViewSet):
    ...

    def get_serializer_class(self):
        if self.action == 'list':
            return SomeSerializer
        else:
            return AnotherSerializer
```

## 11.过滤文章

有些时候用户需要某个特定范围的文章（比如搜索功能），这时候后端需要把返回的数据进行过滤。

最简单的过滤方法就是修改视图集中的 `queryset` 属性了：

```
class ArticleViewSet(viewsets.ModelViewSet):
    queryset = Article.objects.filter(author__username='Obama')
    ...
```

但是这样会导致原本正常的列表也都过滤了，太傻了。因此需要寻找更聪明的办法。

### 参数过滤

假设有如下带有参数的 GET 请求：

```
http://127.0.0.1:8000/api/article/?username=sleepyjoe
```

我们可以覆写 `get_queryset()` 方法来实现过滤：

```python
class ArticleViewSet(viewsets.ModelViewSet):
    queryset = Article.objects.all()
    ...
    
    def get_queryset(self):
        queryset = self.queryset
        username = self.request.query_params.get('username', None)

        if username is not None:
            queryset = queryset.filter(author__username=username)

        return queryset
```

这样就实现了过滤。但是如此常用的功能，必然已经有现成的轮子了。在博客项目中使用轮子可以更快更好的完成任务，所以接下来就来看看通用的过滤功能。

> 注意上面那个请求尾部的斜杠。虽然在浏览器中这个斜杠是可选的，但是在命令行中发送请求是必须要携带的（DRF 3.11.0）。

### 通用过滤

还记得刚安装 DRF 时顺便安装的 `django-filter` 吗，这就是用于过滤的轮子，现在派上用场了。

要将它作为默认的过滤引擎后端，写到配置文件中：

```python
# blog/settings.py

...
REST_FRAMEWORK = {
    'DEFAULT_FILTER_BACKENDS': ['django_filters.rest_framework.DjangoFilterBackend']
    ...
}
```

你也可以将其单独配置在特定的视图中：

```python
# article/views.py
from django_filters.rest_framework import DjangoFilterBackend


class ArticleViewSet(viewsets.ModelViewSet):
    filter_backends = [DjangoFilterBackend]
    filterset_fields = ['author__username', 'title']

    ...
```

最后还要在setting.py中设置

```python
# settings.py
INSTALLED_APPS = [
    ...
    'django_filters',
]
```

如果要实现单纯的完全匹配，上面这样写就可以了，请求参数可以单个字段也可以联合：

 ![image-20230302184002434](assets/image-20230302184002434.png)

如果要实现更常用的**模糊匹配**，就可以使用 `SearchFilter` 做搜索后端：

```python
# article/views.py

...
from rest_framework import filters

class ArticleViewSet(viewsets.ModelViewSet):
    queryset = Article.objects.all()
    serializer_class = ArticleSerializer
    permission_classes = [IsAdminUserOrReadOnly]

    filter_backends = [filters.SearchFilter]
    search_fields = ['title']
    
    # 这个属性不需要了
    # filterset_fields = ['author__username', 'title']
```

**需要注意：**改用模糊匹配后的url参数就只有一个**"search"**，只对一个设置的参数title字段进行搜索

 ![image-20230302184352513](assets/image-20230302184352513.png)

## 12.文章分类

博客文章通常需要分类，方便用户快速识别文章的类型，或者进行某种关联操作。

### 增加分类的模型

首先在 `article/models.py` 里增加一个分类的模型，并且将其和博文成为一对多的外键：

```
# article/models.py

...

class Category(models.Model):
    """文章分类"""
    title = models.CharField(max_length=100)
    created = models.DateTimeField(default=timezone.now)

    class Meta:
        ordering = ['-created']

    def __str__(self):
        return self.title


class Article(models.Model):
    # 分类
    category = models.ForeignKey(
        Category,
        null=True,
        blank=True,
        on_delete=models.SET_NULL,
        related_name='articles'
    )
    ...
```

字段很简单，大体上就 `title` 字段会用到。

别忘了数据迁移：

```sh
(venv) > python manage.py makemigrations
(venv) > python manage.py migrate
```

> 教程把分类的 model 放到 article app中了。实际项目应根据情况考虑是否需要另起一个单独的分类 app。

### 视图与路由

视图还是用视图集的形式：

```python
# article/views.py

...
from article.models import Category
from article.serializers import CategorySerializer

class CategoryViewSet(viewsets.ModelViewSet):
    """分类视图集"""
    queryset = Category.objects.all()
    serializer_class = CategorySerializer
    permission_classes = [IsAdminUserOrReadOnly]
```

与博文的视图集完全一样，没有新的知识。`CategorySerializer` 还没写，不慌等一会来搞定它。

将路由也注册好：

```python
# blog/urls.py

...
from rest_framework.routers import DefaultRouter
from article import views

...
# 其他都不改，就增加这行
router.register(r'category', views.CategoryViewSet)

urlpatterns = [
    ...
]
```

### 序列化器

接下来把 `article/serializers.py` 改成下面这样：

```python
# article/serializers.py

from rest_framework import serializers
from article.models import Article
from user_info.serializers import UserDescSerializer
from article.models import Category


class CategorySerializer(serializers.ModelSerializer):
    """分类的序列化器"""
    url = serializers.HyperlinkedIdentityField(view_name='category-detail')

    class Meta:
        model = Category
        fields = '__all__'
        read_only_fields = ['created']

        
class ArticleSerializer(serializers.HyperlinkedModelSerializer):
    """博文序列化器"""
    author = UserDescSerializer(read_only=True)
    # category 的嵌套序列化字段
    category = CategorySerializer(read_only=True)
    # category 的 id 字段，用于创建/更新 category 外键
    category_id = serializers.IntegerField(write_only=True, allow_null=True, required=False)

    # category_id 字段的验证器
    def validate_category_id(self, value):
        if not Category.objects.filter(id=value).exists() and value is not None:
            raise serializers.ValidationError("Category with id {} not exists.".format(value))
        return value

    class Meta:
        model = Article
        fields = '__all__'
```

稍微开始有点复杂了，让我们来拆分解读一下代码。

先看 `CategorySerializer` ：

- `HyperlinkedIdentityField` 前面章节有讲过，作用是将路由间的表示转换为超链接。`view_name` 参数是路由名，你必须显示指定。 `category-detail` 是自动注册路由时，`Router` 默认帮你设置的详情页面的名称，类似的还有 `category-list` 等，更多规则参考[文档](https://www.django-rest-framework.org/api-guide/routers/#defaultrouter)。
- 创建日期不需要后期修改，所以设置为 `read_only_fields`。

再来看 `ArticleSerializer`：

- 由于我们希望文章接口不仅仅只返回分类的 id 而已，所以需要显式指定 `category` ，将其变成一个嵌套数据，与之前的 `author` 类似。
- DRF 框架原生没有实现**可写的嵌套数据**（因为其操作逻辑没有统一的标准），那我想**创建/更新**文章和分类的外键关系怎么办？一种方法是自己去实现序列化器的 `create()/update()` 方法；另一种就是 DRF 框架提供的修改外键的快捷方式，即显式指定 `category_id` 字段，则此字段会自动链接到 `category` 外键，以便你更新外键关系。
- 再看 `category_id` 内部。`write_only` 表示此字段仅需要可写；`allow_null` 表示允许将其设置为空；`required` 表示在**创建/更新**时可以不设置此字段。

经过以上设置，实际上序列化器已经可以正常工作了。但有个小问题是如果用户提交了一个不存在的分类外键，后端会返回外键数据不存在的 500 错误，不太友好。解决方法就是对数据预先进行**验证**。

验证方式又有如下几种：

- 覆写序列化器的 `.validate(...)` 方法。这是个全局的验证器，其接收的唯一参数是所有字段值的字典。当你需要同时对多个字段进行验证时，这是个很好的选择。
- 另一种就是教程用到的，即 `.validate_{field_name}(...)` 方法，它会只验证某个特定的字段，比如 `category_id` 。

`validate_category_id` 检查了两样东西：

- 数据库中是否包含了对应 id 值的数据。
- 传入值是否为 None。这是为了能够将已有的外键置空。

如果没通过上述检查，后端就抛出一个 400 错误（代替之前的 500 错误），并返回错误产生的提示，这就更友好一些了。

这就基本完成了对分类的开发。接下来就是实际的测试了。

### 测试

打开命令行，首先创建分类：

 ![image-20230302195308496](assets/image-20230302195308496.png)

更新已有的分类：

 ![image-20230302195423145](assets/image-20230302195423145.png)

创建文章时指定分类：

 ![image-20230302195631586](assets/image-20230302195631586.png)

把已有的分类置空：

 ![image-20230302200452627](assets/image-20230302200452627.png)

在更新资源时用到了 `POST` 、`PUT` 、 `PATCH` 三种请求方法，它们的区别是啥？

- `POST` ：创建新的资源。
- `PUT` ： 整体更新特定资源，默认情况下你需要完整给出所有必须的字段。
- `PATCH`： 部分更新特定资源，仅需要给出需要更新的字段，未给出的字段默认不更改。

### 完善分类详情

上面写的分类接口中，我希望分类的**列表页面**不显示其链接的文章以保持数据清爽，但是详情页面则展示出链接的所有文章，方便接口的使用。因此就需要同一个视图集用到两个不同的序列化器了，即前面章节讲的覆写 `get_serializer_class()` 。

修改序列化器：

```python
# article/serializers.py

...

class ArticleCategoryDetailSerializer(serializers.ModelSerializer):
    """给分类详情的嵌套序列化器"""
    url = serializers.HyperlinkedIdentityField(view_name='article-detail')

    class Meta:
        model = Article
        fields = [
            'url',
            'title',
        ]


class CategoryDetailSerializer(serializers.ModelSerializer):
    """分类详情"""
    articles = ArticleCategoryDetailSerializer(many=True, read_only=True)

    class Meta:
        model = Category
        fields = [
            'id',
            'title',
            'created',
            'articles',
        ]
```

然后修改视图：

```python
# article.views.py

...
from article.serializers import CategorySerializer, CategoryDetailSerializer

class CategoryViewSet(viewsets.ModelViewSet):
    """分类视图集"""
    ...
    def get_serializer_class(self):
        if self.action == 'list':
            return CategorySerializer
        else:
            return CategoryDetailSerializer
```

也就是说：

* 通过GET http://127.0.0.1:8000/api/category/时依然会显示分类的url字段
* 通过GET http://127.0.0.1:8000/api/category/1/查看某个指定分类的信息时不会显示url字段，且增加显示该分类下的文章信息

## 13.文章标签

一篇文章通常还有**标签**功能，作为分类的补充。

### 模型视图

老规矩，首先把**标签**的 model 建立好：

```python
# article/models.py

...

class Tag(models.Model):
    """文章标签"""
    text = models.CharField(max_length=30)

    class Meta:
        ordering = ['-id']

    def __str__(self):
        return self.text

...

class Article(models.Model):
    ...
    # 标签
    tags = models.ManyToManyField(
        Tag,
        blank=True,
        related_name='articles'
    )
```

一篇文章可以有多个标签，一个标签可以对应多个文章，因此是**多对多**关系。

接着把视图集也写好：

```python
# article/views.py

...
from article.models import Tag
from article.serializers import TagSerializer

class TagViewSet(viewsets.ModelViewSet):
    queryset = Tag.objects.all()
    serializer_class = TagSerializer
    permission_classes = [IsAdminUserOrReadOnly]
```

还是那三板斧，没有新内容。

最后的外围工作，就是注册路由：

```python
# drf_vue_blog/urls.py

...
router.register(r'tag', views.TagViewSet)
...
```

### 序列化器

接下来就是最重要的 `TagSerializer` ：

```python
# article/serializers.py

...
from article.models import Tag

# 新增的序列化器
class TagSerializer(serializers.HyperlinkedModelSerializer):
    """标签序列化器"""
    class Meta:
        model = Tag
        fields = '__all__'

# 修改已有的文章序列化器
class ArticleSerializer(serializers.HyperlinkedModelSerializer):
    ...
    
    # tag 字段
    tags = serializers.SlugRelatedField(
        queryset=Tag.objects.all(),
        many=True,
        required=False,
        slug_field='text'
    )

    ...
```

通过前面章节已经知道，默认的嵌套序列化器只显示外链的 id，需要改得更友好一些。但似乎又没必要改为超链接或者字段嵌套，因为标签就 `text` 字段有用。因此就用 `SlugRelatedField` 直接显示其 `text` 字段的内容就足够了。

让我们给已有的文章新增一个叫 `python` 的标签试试：

 ![image-20230302204624904](assets/image-20230302204624904.png)

修改失败了，原因是 `python` 标签不存在。多对多关系，DRF 默认你必须先得有这个外键对象，才能指定其关系。虽然也合情合理，但我们更希望在创建、更新文章时，程序会**自动检查**数据库里是否存在当前标签。如果存在则指向它，如果不存在则创建一个并指向它。

要实现这个效果，你可能想到覆写 `.validate_{field_name}()` 或者 `.validate()` 还或者 `.create()/.update()` 方法。但是很遗憾，它们都是不行的。

原因是 DRF 执行默认的字段有效性检查比上述的方法都早，程序还执行不到上述的方法，框架就已经抛出错误了。

正确的解法是覆写 `to_internal_value()` 方法：

```python
# article/serializers.py

...

class ArticleSerializer(serializers.HyperlinkedModelSerializer):
    ...
    # 覆写方法，如果输入的标签不存在则创建它
    def to_internal_value(self, data):
        tags_data = data.get('tags')

        if isinstance(tags_data, list):
            for text in tags_data:
                if not Tag.objects.filter(text=text).exists():
                    Tag.objects.create(text=text)

        return super().to_internal_value(data)
```

`to_internal_value()` 方法原本作用是将请求中的原始 Json 数据转化为 Python 表示形式（期间还会对字段有效性做初步检查）。它的执行时间比默认验证器的字段检查更早，因此有机会在此方法中将需要的数据创建好，然后等待检查的降临。`isinstance()` 确定标签数据是列表，才会循环并创建新数据。

再重新请求试试：

 ![image-20230302205731921](assets/image-20230302205731921.png)

这次成功了。可以看到同时赋值多个标签也是可以的，置空也是可以的（给个空列表）。

除此之外，因为标签仅有 `text` 字段是有用的，两个 `id` 不同但是 `text` 相同的标签没有任何意义。更重要的是，`SlugRelatedField` 是不允许有重复的 `slug_field` 。因此还需要覆写 `TagSerializer` 的 `create()/update()` 方法：

```python
# article/serializers.py

...

class TagSerializer(serializers.HyperlinkedModelSerializer):
    """标签序列化器"""

    def check_tag_obj_exists(self, validated_data):
        text = validated_data.get('text')
        if Tag.objects.filter(text=text).exists():
            raise serializers.ValidationError('Tag with text {} exists.'.format(text))

    def create(self, validated_data):
        self.check_tag_obj_exists(validated_data)
        return super().create(validated_data)

    def update(self, instance, validated_data):
        self.check_tag_obj_exists(validated_data)
        return super().update(instance, validated_data)
    
    ...
```

这样就防止了重复 `text` 的标签对象出现。

这两个序列化器的完整形态是下面这样子的：

```python
# article/serializers.py

class TagSerializer(serializers.HyperlinkedModelSerializer):
    """标签序列化器"""

    def check_tag_obj_exists(self, validated_data):
        text = validated_data.get('text')
        if Tag.objects.filter(text=text).exists():
            raise serializers.ValidationError('Tag with text {} exists.'.format(text))

    def create(self, validated_data):
        self.check_tag_obj_exists(validated_data)
        return super().create(validated_data)

    def update(self, instance, validated_data):
        self.check_tag_obj_exists(validated_data)
        return super().update(instance, validated_data)

    class Meta:
        model = Tag
        fields = '__all__'


class ArticleSerializer(serializers.HyperlinkedModelSerializer):
    """博文序列化器"""
    author = UserDescSerializer(read_only=True)
    # category 的嵌套序列化字段
    category = CategorySerializer(read_only=True)
    # category 的 id 字段，用于创建/更新 category 外键
    category_id = serializers.IntegerField(write_only=True, allow_null=True, required=False)
    # tag 字段
    tags = serializers.SlugRelatedField(
        queryset=Tag.objects.all(),
        many=True,
        required=False,
        slug_field='text'
    )

    # 覆写方法，如果输入的标签不存在则创建它
    def to_internal_value(self, data):
        tags_data = data.get('tags')

        if isinstance(tags_data, list):
            for text in tags_data:
                if not Tag.objects.filter(text=text).exists():
                    Tag.objects.create(text=text)

        return super().to_internal_value(data)

    # category_id 字段的验证器
    def validate_category_id(self, value):
        # 数据存在且传入值不等于None
        if not Category.objects.filter(id=value).exists() and value != None:
            raise serializers.ValidationError("Category with id {} not exists.".format(value))

        return value

    class Meta:
        model = Article
        fields = '__all__'
```

## 14.Markdown正文

博客文章需要排版，否则难以凸显标题、正文、注释等内容之间的区别。作为博客写手来说，比较流行且好用的排版是采用 Markdown 语法。

严格来说， Markdown 是一种排版标注规则。它将两个星号包裹的文字标注为重要文本（通常也就是粗体字），比如原始文本中的 `**Money**` ，在 Markdown 语法中应该被”渲染“为粗体，也就是 **Money** 。类似的还有斜体、代码块、表格、公式等注释，就请读者自行了解了。

> [关于 Markdown](https://www.dusaiphoto.com/article/20/) 的简单介绍。

”渲染“ Markdown 也就是把原始文本中的注释转化为前端中真正被用户看到的 HTML 排版文字。渲染过程可以在前端也可以在后端，本文将使用后端渲染，以便你理解 DRF 的相关知识。

### 模型和视图

为了将博文的 Markdown 正文渲染为 html 标签，首先给文章模型添加一个 `get_md()` 方法：

```python
# article/models.py

from markdown import Markdown
...

class Article(models.Model):
    ...
    
    # 新增方法，将 body 转换为带 html 标签的正文
    def get_md(self):
        md = Markdown(
            extensions=[
                'markdown.extensions.extra',
                'markdown.extensions.codehilite',
                'markdown.extensions.toc',
            ]
        )
        md_body = md.convert(self.body)
        # toc 是渲染后的目录
        return md_body, md.toc
```

方法返回了包含了两个元素的元组，分别为已渲染为 html 的**正文**和**目录**。

这些渲染后的数据，在文章**详情接口**是需要的，但是在**列表接口**却没太有必要，因此又要用到视图集根据请求方式动态获取序列化器的技术了：

```python
# article/views.py

from article.serializers import ArticleDetailSerializer

...

# 新增 get_serializer_class() 方法
class ArticleViewSet(viewsets.ModelViewSet):
    ...

    def get_serializer_class(self):
        if self.action == 'list':
            return ArticleSerializer
        else:
            return ArticleDetailSerializer
```

序列化器 `ArticleDetailSerializer` 还没有写

### 序列化器

因为**文章列表接口**和**详情接口**只有一点点返回字段的区别，其实大部分功能还是一样的。所以抽象成为父类：

```python
# article/serializers.py

...

# 将已有的 ArticleSerializer 里的东西全部挪到这个 ArticleBaseSerializer 里来
# 除了 Meta 类保留
class ArticleBaseSerializer(serializers.HyperlinkedModelSerializer):
    author = ...
    category = ...
    category_id = ...
    tags = ...

    def to_internal_value(self, data):
        ...

    def validate_category_id(self, value):
        ...

# 保留 Meta 类
# 将父类改为 ArticleBaseSerializer
class ArticleSerializer(ArticleBaseSerializer):
    class Meta:
        model = Article
        fields = '__all__'
        extra_kwargs = {'body': {'write_only': True}}
```

与 Django 表单类似，你可以继承扩展和重用序列化器。就像上面的代码一样，在父类上声明一组通用的字段或方法，然后在许多序列化程序中使用它们。

但是内部类 `class Meta` 比较特殊，它不会隐式从父类继承。虽然有办法让它隐式继承，但这是不被推荐的，你应该显式声明它，以使得序列化器的行为更清晰。

另外，如果你觉得在列表接口连 `body` 字段也不需要显示的话，你可以传入 `extra_kwargs` 使其变成仅可写却不显示的字段。

把这些**代码重构**的准备工作都搞定之后，就可以正式写这个新的 `ArticleDetailSerializer` 了：

```python
# article/serializers.py

...

# 注意继承的父类是 ArticleBaseSerializer
class ArticleDetailSerializer(ArticleBaseSerializer):
    # 渲染后的正文
    body_html = serializers.SerializerMethodField()
    # 渲染后的目录
    toc_html = serializers.SerializerMethodField()

    def get_body_html(self, obj):
        return obj.get_md()[0]

    def get_toc_html(self, obj):
        return obj.get_md()[1]

    class Meta:
        model = Article
        fields = '__all__'
```

`body_html` 、 `toc_html` 这两个渲染后的字段是经过加工后的数据，不存在于原始的数据中。为了将这类只读的附加字段添加到接口里，就可以用到 `SerializerMethodField()` 字段了。比如说上面代码中的 `body_html` 字段，它会自动去调用 `get_body_html()` 方法，并将其返回结果作为需要序列化的数据。方法中的 `obj` 参数是序列化器获取到的 model 实例，也就是文章对象了。

这样就大功告成了，读者自己测试一下，顺利的话详情接口就可以返回 Markdown 渲染后的数据了。

 ![image-20230302224810408](assets/image-20230302224810408.png)

> 记得原始文本应该用 Markdown 语法编写。成功的话 `body_html` 字段返回的是带有 html 标签的文本。
>
> 代码重构得太早可能会导致某些不必要的抽象，太晚又可能堆积太多”屎山“而无从下手。理想情况下的重构是随着项目的开发同时进行的，在合适的节点进行合适的抽象，看着代码逐渐规整，你也会相当有成就感。

另一个问题是，有时候你可能出于版权方面的考虑不愿意将原始的 Markdown 文章数据给任意用户，那么这里只要做一次鉴权，根据用户的权限选用不同的序列化器即可。（非管理员不返回原始文章数据）

## 15.文章标题图

即使是一个最简单的博客项目，也绕不开文件的上传与下载，比如说博文的标题图片。很遗憾，Json 格式的载体是字符串，不能够直接处理文件流。

怎么办？很多开发者用 DRF 处理文件上传还是沿用了 Django 的老路子，即用 `multipart/form-data` 表单发送夹杂着元数据的文件。这种方法可行，但在主要接口中发送编码文件总感觉不太舒服。

除了上面这种老路子以外，你基本上还剩三种选择：

- 用 Base64 对文件进行编码（将文件变成字符串）。这种方法简单粗暴，并且只靠 Json 接口就可以实现。代价是数据传输大小增加了约 33％，并在服务器和客户端中增加了编码/解码的开销。
- 首先在 `multipart/form-data` 中单独发送文件，然后后端将保存好的文件 id 返回给客户端。客户端拿到文件 id 后，发送带有文件 id 的 Json 数据，在服务器端将它们关联起来。
- 首先单独发送 Json 数据，然后后端保存好这些元数据后将其 id 返回给客户端。接着客户端发送带有元数据 id 的文件，在服务器端将它们关联起来。

三种方法各有优劣，具体用哪种方法应当视实际情况确定。

本文将使用第二种方法来实现博文标题图的功能。

### 模型和视图

图片字段 `ImageField` 依赖 `Pillow` 库，先把它安装好：

```
python -m pip install Pillow
```

> 旧版本 pip 可能安装 Pillow 会失败，比如 pip==10.x 。如果安装过程中报错，请尝试升级 pip。

按照上述两步走的思路：先上传图片、再上传其他文章数据的流程，将标题图设计为一个独立的模型：

```python
# article/models.py

...
class Avatar(models.Model):
    content = models.ImageField(upload_to='avatar/%Y%m%d')


class Article(models.Model):
    ...
    # 标题图
    avatar = models.ForeignKey(
        Avatar,
        null=True,
        blank=True,
        on_delete=models.SET_NULL,
        related_name='article'
    )
```

`Avatar` 模型仅包含一个图片字段。接收的图片将保存在 `media/avatar/年月日/` 的路径中。

接着按部就班的把视图集写了：

```python
# article/views.py

...
from article.models import Avatar
# 这个 AvatarSerializer 最后来写
from article.serializers import AvatarSerializer

class AvatarViewSet(viewsets.ModelViewSet):
    queryset = Avatar.objects.all()
    serializer_class = AvatarSerializer
    permission_classes = [IsAdminUserOrReadOnly]
```

图片属于媒体文件，它也需要路由，因此会多一点点配置工作：

```python
# blog/settings.py

...
MEDIA_URL =  '/media/'
MEDIA_ROOT = os.path.join(BASE_DIR, 'media')
```

以及注册路由：

```python
# blog/urls.py

...
from django.conf import settings
from django.conf.urls.static import static

...
router.register(r'avatar', views.AvatarViewSet)

urlpatterns = [
    ...
]

# 把媒体文件的路由注册了
if settings.DEBUG:
  urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
```

这些准备工作都搞好了，就又到了喜闻乐见的写序列化器的环节。

### 序列化器

图片是在文章上传前先单独上传的，因此需要有一个单独的序列化器：

```python
# article/serializers.py

...

from article.models import Avatar

class AvatarSerializer(serializers.ModelSerializer):
    url = serializers.HyperlinkedIdentityField(view_name='avatar-detail')

    class Meta:
        model = Avatar
        fields = '__all__'
```

DRF 对图片的处理进行了封装，通常不需要你关心实现的细节，只需要像其他 Json 接口一样写序列化器就可以了。

图片上传完成后，会将其 id、url 等信息返回到前端，前端将图片的信息以嵌套结构表示到文章接口中，并在适当的时候将其链接到文章数据中：

```python
# article/serializers.py

...

class ArticleBaseSerializer(serializers.HyperlinkedModelSerializer):
    ...
    
    # 图片字段
    avatar = AvatarSerializer(read_only=True)
    avatar_id = serializers.IntegerField(
        write_only=True, 
        allow_null=True, 
        required=False
    )

    # 验证图片 id 是否存在
    # 不存在则返回验证错误
    def validate_avatar_id(self, value):
        if not Avatar.objects.filter(id=value).exists() and value is not None:
            raise serializers.ValidationError("Avatar with id {} not exists.".format(value))

        return value
    
    ...
```

用户的操作流程如下：

- 发表新文章时，标题图需要先上传。
- 标题图上传完成会返回其数据（比如图片数据的 id）到前端并暂存，等待新文章完成后一起提交。
- 提交新文章时，序列化器对标题图进行检查，如果无效则返回错误信息。

> 这个流程在后面的前端章节会体现得更直观。

### 测试

接下来测试图片的增删改查。

> Postman 操作文件接口需要将 `Content-Type` 改为 `multipart/form-data` ，并在 `Body` 中上传图片文件。具体操作方式请百度。

创建新图片：

 ![image-20230302232139054](assets/image-20230302232139054.png)

> 看到创建图片后返回的 id 了吗？其实就是图片是先于 Json 数据单独上传的，上传完毕后客户端将其 id 记住，以便真正提交 Json 时能与之对应。

更新已有图片：

 ![image-20230302232334761](assets/image-20230302232334761.png)

删除：

![image-20230302232545098](assets/image-20230302232545098.png)

查找图片：

 ![image-20230302232457631](assets/image-20230302232457631.png)

### 重构

仔细看下 `ArticleBaseSerializer` 序列化器，发现**分类**和**标题图**的验证方法是比较类似的：

```python
# article/serializers.py

...

class ArticleBaseSerializer(serializers.HyperlinkedModelSerializer):
    ...

    def validate_avatar_id(self, value):
        if not Avatar.objects.filter(id=value).exists() and value is not None:
            raise serializers.ValidationError("Avatar with id {} not exists.".format(value))
            self.fail('incorrect_avatar_id', value=value)

        return value

    def validate_category_id(self, value):
        if not Category.objects.filter(id=value).exists() and value is not None:
            raise serializers.ValidationError("Category with id {} not exists.".format(value))
            self.fail('incorrect_category_id', value=value)

        return value
```

因此可以将它们整理整理，变成下面的样子：

```python
# article/serializers.py

...

class ArticleBaseSerializer(serializers.HyperlinkedModelSerializer):
    ...

    # 自定义错误信息
    default_error_messages = {
        'incorrect_avatar_id': 'Avatar with id {value} not exists.',
        'incorrect_category_id': 'Category with id {value} not exists.',
        'default': 'No more message here..'
    }

    def check_obj_exists_or_fail(self, model, value, message='default'):
        if not self.default_error_messages.get(message, None):
            message = 'default'

        if not model.objects.filter(id=value).exists() and value is not None:
            self.fail(message, value=value)

    def validate_avatar_id(self, value):
        self.check_obj_exists_or_fail(
            model=Avatar,
            value=value,
            message='incorrect_avatar_id'
        )

        return value

    def validate_category_id(self, value):
        self.check_obj_exists_or_fail(
            model=Category,
            value=value,
            message='incorrect_category_id'
        )

        return value
```

- 把两个字段验证器的雷同代码抽象到 `check_obj_exists_or_fail()` 方法里。
- `check_obj_exists_or_fail()` 方法检查了数据对象是否存在，若不存在则调用钩子方法 `fail()` 引发错误。
- `fail()` 又会调取 `default_error_messages` 属性中提供的错误类型，并将其返回给接口。

看起来似乎代码行数更多了，但更整洁了。起码你的报错信息不再零散分布在整个序列化器中，并且合并了两个验证器的重复代码，维护起来会更省事。

## 16.评论

评论是博客作者和读者进行沟通的重要方式，也是博客作者检视自身文章质量的手段。

虽然有很多方式可以将评论功能托管给第三方（我也推荐这么做），不过本着学习的目的，接下来就试着自己实现简单的评论接口。

### 准备工作

评论功能比较独立，因此另起一个 `comment` 的 App：

```
(venv) > python manage.py startapp comment
```

注册到配置文件：

```python
# drf_vue_blog/settings.py

...

INSTALLED_APPS = [
    ...
    'comment',
]
```

接下来就是模型：

```python
# comment/models.py

from django.db import models
from django.utils import timezone

from article.models import Article
from django.contrib.auth.models import User


class Comment(models.Model):
    author = models.ForeignKey(
        User,
        on_delete=models.CASCADE,
        related_name='comments'
    )

    article = models.ForeignKey(
        Article,
        on_delete=models.CASCADE,
        related_name='comments'
    )

    content = models.TextField()
    created = models.DateTimeField(default=timezone.now)

    class Meta:
        ordering = ['-created']

    def __str__(self):
        return self.content[:20]
```

模型包含一对多的作者外键、一对多的文章外键、评论实际内容、评论时间这4个字段。

执行 `makemigrations` 和 `migrate` ，准备工作就完成了。

### 视图和序列化

视图集和之前章节的差不多：

```python
# comment/views.py

from rest_framework import viewsets

from comment.models import Comment
from comment.serializers import CommentSerializer
from comment.permissions import IsOwnerOrReadOnly


class CommentViewSet(viewsets.ModelViewSet):
    queryset = Comment.objects.all()
    serializer_class = CommentSerializer
    permission_classes = [IsOwnerOrReadOnly]

    def perform_create(self, serializer):
        serializer.save(author=self.request.user)
```

接下来写评论的权限。

评论对用户身份的要求比文章的更松弛，**非安全请求**只需要是本人操作就可以了。

因此自定义一个所有人都可查看、仅本人可更改的权限：

```python
# comment/permissions.py

from rest_framework.permissions import BasePermission, SAFE_METHODS

class IsOwnerOrReadOnly(BasePermission):
    message = 'You must be the owner to update.'

    def has_permission(self, request, view):
        if request.method in SAFE_METHODS:
            return True

        return request.user.is_authenticated

    def has_object_permission(self, request, view, obj):
        if request.method in SAFE_METHODS:
            return True

        return obj.author == request.user
```

进行**非安全请求**时，由于需要验证当前评论的作者和当前登录的用户是否为同一个人，这里用到了 `def has_object_permission(...)` 这个钩子方法，方法参数中的 `obj` 即为评论模型的实例。

看起来只需要实现这个 `def has_object_permission(...)` 就可以了，但还有一点点小问题：此方法是晚于视图集中的 `def perform_create(author=self.request.user)` 执行的。如果用户未登录时新建评论，由于用户不存在，接口会抛出 500 错误。

本着即使出错也要做出正确错误提示的原则，增加了 `def has_permission(...)` 方法。此方法早于 `def perform_create(...)` 执行，因此能够对用户登录状态做一个预先检查。

功能这样就实现了，但是重复的代码又出现了，让我们来消灭它。

删掉旧代码，把这个权限类修改为下面这样：

```python
# comment/permissions.py
# ...
class IsOwnerOrReadOnly(BasePermission):
    message = 'You must be the owner to update.'

    def safe_methods_or_owner(self, request, func):
        if request.method in SAFE_METHODS:
            return True

        return func()

    def has_permission(self, request, view):
        return self.safe_methods_or_owner(
            request,
            lambda: request.user.is_authenticated
        )

    def has_object_permission(self, request, view, obj):
        return self.safe_methods_or_owner(
            request,
            lambda: obj.author == request.user
        )
```

用匿名函数将有函数体（闭包）作为参数，传递到 `def safe_methods_or_owner(...)` 方法里执行，效果和之前是完全一样的。

接下来的东西就都轻车熟路了。

将视图集注册到路由：

```python
# blog/urls.py

...

# 这里直接导入 views 会冲突
from comment.views import CommentViewSet
router.register(r'comment', CommentViewSet)
```

将评论的序列化器写了：

```python
# comment/serializers.py

from rest_framework import serializers

from comment.models import Comment
from user_info.serializers import UserDescSerializer


class CommentSerializer(serializers.ModelSerializer):
    url = serializers.HyperlinkedIdentityField(view_name='comment-detail')
    author = UserDescSerializer(read_only=True)

    class Meta:
        model = Comment
        fields = '__all__'
        extra_kwargs = {'created': {'read_only': True}}
```

跟之前一样， `url` 超链接字段让接口的跳转更方便，`author` 嵌套序列化器让显示的内容更丰富。

最后让评论通过文章接口显示出来：

```python
# article/serializers.py

...

from comment.serializers import CommentSerializer

class ArticleDetailSerializer(...):
    id = serializers.IntegerField(read_only=True)
    comments = CommentSerializer(many=True, read_only=True)

    ...
```

### 测试

发几个请求测试接口逻辑是否正确。

未登录用户新建评论：

 ![image-20230303190717412](assets/image-20230303190717412.png)

用之前注册好的用户 `sleepyjoe` 新建评论：

 ![image-20230303190908324](assets/image-20230303190908324.png)

用非本人用户 `cy99` 更新评论：

 ![image-20230303191305157](assets/image-20230303191305157.png)

用 `sleepyjoe` 删除评论：

![image-20230303191436262](assets/image-20230303191436262.png)

非本人无法对资源进行更改，很好的符合了预期逻辑。

## 17.二级评论

上一章我们做好了评论功能，就有了作者和读者沟通的方式。

但有的时候读者和读者同样需要沟通，评论别人的评论，俗称**多级评论**。

本章将实现基础的多级评论功能。

> 准确的讲是两级评论。

### 模型

多级评论，也就是让评论模型和自身相关联，使其可以有一个父级。

修改评论模型，新增 `parent` 字段：

```python
# comment/models.py

...
class Comment(models.Model):
    ...
    parent = models.ForeignKey(
        'self',
        null=True,
        blank=True,
        on_delete=models.SET_NULL,
        related_name='children'
    )
```

- 一个父评论可以有多个子评论，而一个子评论只能有一个父评论，因此用了一对多外键。
- 之前的一对多外键，第一个参数直接引用了对应的模型，但是由于语法规则限制，这里显然不能够自己引用自己，因此用了传递字符串 `self` 的方式，作用都是一样的。

进行迁移后，模型就改好了。

### 序列化器

在原有的评论序列化器上修改：

```python
# comment/serializers.py

...

# 新增这个类
class CommentChildrenSerializer(serializers.ModelSerializer):
    url = serializers.HyperlinkedIdentityField(view_name='comment-detail')
    author = UserDescSerializer(read_only=True)

    class Meta:
        model = Comment
        exclude = [
            'parent',
            'article'
        ]

# 修改这个类
class CommentSerializer(serializers.ModelSerializer):
    # 这是已有代码
    url = serializers.HyperlinkedIdentityField(view_name='comment-detail')
    author = UserDescSerializer(read_only=True)

    # 以下是新增代码
    article = serializers.HyperlinkedRelatedField(view_name='article-detail', read_only=True)
    article_id = serializers.IntegerField(write_only=True, allow_null=False, required=True)

    parent = CommentChildrenSerializer(read_only=True)
    parent_id = serializers.IntegerField(write_only=True, allow_null=True, required=False)

    def update(self, instance, validated_data):
        validated_data.pop('parent_id', None)
        return super().update(instance, validated_data)

    class Meta:
        ...
```

新增代码大致可以分为三块，让我们来拆解它们：

- 为了让文章引用更人性化，将 `article` 改为超链接字段用了 `HyperlinkedRelatedField` ，它和之前用过的 `HyperlinkedIdentityField` 差别很小，你可以简化理解为 `HyperlinkedRelatedField` 用于对外键关系，而 `HyperlinkedIdentityField` 用于对当前模型自身。（完整的解释[看这里](https://stackoverflow.com/questions/31566675/for-django-rest-framework-what-is-the-difference-in-use-case-for-hyperlinkedrel)）
- `parent` 为父评论，用了嵌套序列化器 `CommentChildrenSerializer` 。注意这个序列化器的 `Meta` 用 `exclude` 来定义不需要的字段。
- 由于我们希望父评论只能在创建时被关联，后续不能更改（很合理），因此覆写 `def update(...)` ，使得在更新评论时忽略掉 `parent_id` 参数。

这就完成了。接下来测试。

### 测试

新建一个文章主键为 2 、父评论主键为 7 的评论：

![image-20230303193848536](assets/image-20230303193848536.png)

注意这里由于将 `article` 改为了嵌套序列化器（只读），因此用 `article_id` 进行外键赋值。

如果我想在更新评论内容的同时修改父评论：

![image-20230303194151909](assets/image-20230303194151909.png)

`content` 修改成功而 `parent_id` 无变化，和预想的逻辑表现一致。

其他的请求形式就不赘述了，读者可以自行尝试。

## 18.JWT身份验证

Web 程序是使用 HTTP 协议传输的，而 HTTP 协议是**无状态**的协议，对于事务没有记忆能力。也就是说，如果没有其他形式的帮助，服务器是没办法知道前后两次请求是否是同一个用户发起的，也不具有对用户进行身份验证的能力。

传统 web 开发中（以及前面的章节），身份验证**通常**是基于 Session 会话机制的。Session 对象存储特定用户会话所需的属性及配置信息。这样，当用户在应用程序的 Web 页之间跳转时，存储在 Session 对象中的变量将不会丢失，而是在整个用户会话中一直存在下去。 Session 通常是存储在服务器当中的，如果 Session 过多，会对服务器产生压力。

另一种比较常用的身份验证方式是 JWT (JSON Web Token) 令牌。JWT 是一种开放标准，它定义了一种紧凑且自包含的方式，用于在各方之间作为 JSON 对象安全地传输信息。由于 Token 是经过数字签名的，因此可以被验证和信任。JWT 非常适合用于身份验证和服务器到服务器授权。与 Session 不同，JWT 的 **Token** 是保存在用户端的，即摆脱了对服务器的依赖。

JWT 令牌长这样子的：

```
eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ0b2tlbl90eXBlIjoiYWNjZXNzIiwiZXhwIjoxNjA5MjEwMjg0LCJqdGkiOiJiNzMxMTliMTZjNWM0YTExODNkOGJiZTNhZDZmZmYzMyIsInVzZXJfaWQiOjJ9.59ZavqFzjE3EoDrniu2cwgc_cq1Rv1OxpZeqRte_HLw
```

在进行某些需要验证身份的业务中，用户需要把令牌一并提交（就跟提交用户名及密码类似）。

> 这里有详细的 [JWT 工作方式讲解](https://jwt.io/introduction)。

本章尝试将身份验证方法更改为 JWT 形式。

### 代码修改

首先 pip 安装 `djangorestframework-simplejwt` 这个 jwt 库：

```
(venv) > pip install djangorestframework-simplejwt
```

修改配置文件，使 JWT 为默认的验证机制：

```python
# blog/settings.py

...

REST_FRAMEWORK = {
    ...
    
    'DEFAULT_AUTHENTICATION_CLASSES': (
        'rest_framework_simplejwt.authentication.JWTAuthentication',
    )

}
```

在根路由中添加 Token 的获取和刷新地址：

```python
# blog/urls.py

...

from rest_framework_simplejwt.views import (
    TokenObtainPairView,
    TokenRefreshView,
)

urlpatterns = [
    ...
    path('api/token/', TokenObtainPairView.as_view(), name='token_obtain_pair'),
    path('api/token/refresh/', TokenRefreshView.as_view(), name='token_refresh'),
]
```

这就完成了，毫无痛苦，这就是用一个优秀轮子的好处。

### 测试

上一章的评论模块已经做好了权限控制的内容，正好拿来测试。

首先，携带用户名和密码发送一个 POST 请求，以获取 Token：

![image-20230303200501177](assets/image-20230303200501177.png)

拿到 Token 后，就可以用 Token 作为你的身份令牌，进行正常的资源请求了：

> Postman 有一个专门的标签页 (Authorization) 用于填写令牌。此标签页的 Type 栏选择 Bearer Token 即可。

![image-20230303201341576](assets/image-20230303201341576.png)

令牌具有过期时间（默认为**5分钟**，可在配置中修改），过期之后就不能再使用了，但是可用刷新令牌再获取一个新的令牌：

![image-20230303201639882](assets/image-20230303201639882.png)

功能与用 Session 相同，并且成功切换到 JWT 方式了。

> 开启 JWT 后，Session 验证就自动失效了。也就是说，除了申请 Token 时会用到账户密码，其他时候的身份验证都不再需要它们了。
>
> Session 和 JWT 哪个好？将会话移至客户端意味着摆脱了对服务器端会话的依赖，但这会带来如何安全存储、运输令牌等一系列挑战。不能够一概而论，而是要根据你的项目实际需求。关于这个话题更深入的讨论，请移步[Stackoverflow](https://stackoverflow.com/questions/43452896/authentication-jwt-usage-vs-session)。

### 有效期

Token 默认有效期很短，只有 5 分钟。你可以通过修改 Django 的配置文件进行更改：

```python
# blog/settings.py

...

from datetime import timedelta

SIMPLE_JWT = {
    'ACCESS_TOKEN_LIFETIME': timedelta(days=1),
    'REFRESH_TOKEN_LIFETIME': timedelta(days=10),
}
```

> Token 一旦泄露，任何人都可以获得该令牌的所有权限。出于安全考虑，Token 的有效期通常不应该设置得太长。

更多配置项请查看[官方文档](https://django-rest-framework-simplejwt.readthedocs.io/en/latest/settings.html)。

## 19.用户管理

上一章搞定了 JWT 登录，本章接着来实现用户信息的增删改查。

### 用户管理

用户管理涉及到对密码的操作，因此新写一个序列化器，覆写 `def create(...)` 和 `def update(...)` 方法：

```python
# user_info/serializers.py

...

class UserRegisterSerializer(serializers.ModelSerializer):
    url = serializers.HyperlinkedIdentityField(view_name='user-detail', lookup_field='username')

    class Meta:
        model = User
        fields = [
            'url',
            'id',
            'username',
            'password'
        ]
        extra_kwargs = {
            'password': {'write_only': True}
        }

    def create(self, validated_data):
        user = User.objects.create_user(**validated_data)
        return user

    def update(self, instance, validated_data):
        if 'password' in validated_data:
            password = validated_data.pop('password')
            instance.set_password(password)
        return super().update(instance, validated_data)
```

- 注意 `def update(...)` 时，密码需要单独拿出来通过 `set_password()` 方法加密后存入数据库，而不能以明文的形式保存。
- 超链接字段的参数有一条 `lookup_field`，这是指定了解析超链接关系的字段。直观来说，将其配置为 `username` 后，用户详情接口的地址表示为用户名而不是主键。

用户管理同样涉及的权限问题，因此新建 `permissions.py`，写入代码：

```python
# user_info/permissions.py

from rest_framework.permissions import BasePermission, SAFE_METHODS


class IsSelfOrReadOnly(BasePermission):

    def has_object_permission(self, request, view, obj):
        if request.method in SAFE_METHODS:
            return True

        return obj == request.user
```

这个权限类和之前写过的类似，确保非安全方法只能由本人操作。

铺垫工作做好了，最后就是写视图集：

```python
# user_info/views.py

from django.contrib.auth.models import User
from rest_framework import viewsets
from rest_framework.permissions import AllowAny, IsAuthenticatedOrReadOnly

from user_info.serializers import UserRegisterSerializer
from user_info.permissions import IsSelfOrReadOnly


class UserViewSet(viewsets.ModelViewSet):
    queryset = User.objects.all()
    serializer_class = UserRegisterSerializer
    lookup_field = 'username'

    def get_permissions(self):
        if self.request.method == 'POST':
            self.permission_classes = [AllowAny]
        else:
            self.permission_classes = [IsAuthenticatedOrReadOnly, IsSelfOrReadOnly]

        return super().get_permissions()
```

- 注册用户的 POST 请求是允许所有人都可以操作的，但其他类型的请求（比如修改、删除）就必须是本人才行了，因此可以覆写 `def get_permissions(...)` 定义不同情况下所允许的权限。 `permission_classes` 接受列表，因此可以同时定义多个权限，权限之间是 and 关系。
- 注意这里的 `lookup_field` 属性，和序列化器中对应起来。

接着注册路由：

```python
# blog/urls.py

...

from user_info.views import UserViewSet

router.register(r'user', UserViewSet)

...
```

用户的增删改查就完成了。可见 DRF 封装层级很高，常规功能完全隐藏在框架之中了。

试着发送一个 get 请求：

![image-20230303215705848](assets/image-20230303215705848.png)

可以看到详情地址不是主键值而是用户名了，这就是 `lookup_field` 发挥的作用。

 修改密码：![image-20230303221236376](assets/image-20230303221236376.png)

### 自定义动作

视图集除了默认的增删改查外，还可以有其他的自定义动作。

为了测试，首先写一个信息更加丰富的用户序列化器：

```python
# user_info/serializers.py

...

class UserDetailSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = [
            'id',
            'username',
            'last_name',
            'first_name',
            'email',
            'last_login',
            'date_joined'
        ]
```

接着就可以在视图集中新增代码，自定义动作了：

```python
# user_info/views.py

...

from rest_framework.decorators import action
from rest_framework.response import Response
from user_info.serializers import UserDetailSerializer

class UserViewSet(viewsets.ModelViewSet):
    ...

    @action(detail=True, methods=['get'])
    def info(self, request, username=None):
        queryset = User.objects.get(username=username)
        serializer = UserDetailSerializer(queryset, many=False)
        return Response(serializer.data)

    @action(detail=False)
    def sorted(self, request):
        users = User.objects.all().order_by('-username')

        page = self.paginate_queryset(users)
        if page is not None:
            serializer = self.get_serializer(page, many=True)
            return self.get_paginated_response(serializer.data)

        serializer = self.get_serializer(users, many=True)
        return Response(serializer.data)
```

魔法都在装饰器 `@action` 里，它的参数可以定义是否为详情的动作、请求类型、url 地址、url 解析名等常规需求。

### 测试

获取单个用户详情：

![image-20230303222310126](assets/image-20230303222310126.png)

排序并获取用户列表:

![image-20230303222348530](assets/image-20230303222348530.png)

默认情况下，方法名就是此动作的路由路径。返回的 Json 也正确显示为方法中所封装的数据。

> 关于自定义动作详见[官方文档](https://www.django-rest-framework.org/api-guide/viewsets/#marking-extra-actions-for-routing)。

## 20.前端开发准备

博客后端的主要开发工作就告一段落了。

**覆盖的知识点**主要有：

- REST 的概念
- 序列化器/APIView
- 类视图和通用视图
- 权限控制
- ModelSerializer
- 超链接字段/分页
- 视图集/ModelViewSet
- SearchFilter/DjangoFilterBackend
- 嵌套序列化器/操作外键
- 验证器/覆写动作
- 自定义字段
- 文件上传
- 添加保存数据
- 模型自关联
- JWT 身份验证
- 权限分配/自定义动作

后面的章节将正式进入 Vue 前端的搭建。**核心知识点**有：

- 用 Vue 搭建前端页面
- 实现 Vue 和 Django 的配合
- 继续完善 Django 后端功能

在前端开发中，笔者将用到的环境如下：

- Vue 3
- Vue-Cli 5.0.8
- npm 6.14.12

### 准备工作

开发前端时会用到 **npm**（类似 Python 的包管理工具 Pip），这是 Node.js 官方提供的包管理工具。

所以准备工作的第一步，安装 Node.js，下载地址在[官网](https://nodejs.org/en/)，安装时基本就是一路 next。

完毕后打开命令行（依旧默认是 PowerShell），输入：

```
> npm -v
6.14.12
```

显示版本号就表示安装成功了。

> npm 站点在国外，如果你遇到安装速度慢的问题，可以用指令 `npm config set registry https://registry.npm.taobao.org` 修改为国内镜像源。

接下来就可以安装 Vue 的**命令行工具**，它可以帮助我们方便的搭建 Vue 项目的骨架：

```
> npm install -g @vue/cli

# 这里省略一段神秘的安装文字...

> vue --version
@vue/cli 5.0.8
```

同样的，显示版本号就表示安装成功了。

> 深入了解见[Vue-Cli文档](https://cli.vuejs.org/zh/guide/)。

> 如果运行命令报错"vue: 无法加载文件 C:\xxx\vue.ps1，因为在此系统上禁止运行脚本"，则需要通过 PowerShell 解除 `Execution_Policies` 运行策略限制。方法见[这里](https://blog.csdn.net/moshowgame/article/details/109405127)。

进入 Django 项目的根目录，用命令行工具搭建 Vue 骨架：

```
# 改为你的项目根路径
> cd D:\develop\git_repo\GitHub_cy\Cymoonlight\blog
> vue create frontend
```

**一定要**选择安装 Vue 3：

> 前面说了，Vue 3 和 Vue 2 变化比较大，装错了后面章节的代码可能都跑不起来。

然后等待安装完成：

 ![image-20230303223641903](assets/image-20230303223641903.png)

出现这段文字说明 Vue 安装完成了。

与 Django 需要运行服务器类似，作为前后端分离的项目，**在开发时**前端同样也需要运行前端的服务器。

根据文字提示，进入 `frontend` 目录，运行 Vue 的开发服务器：

 ![image-20230303225504984](assets/image-20230303225504984.png)

`http://localhost:8080/` 即可看到 Vue 的欢迎页面了。

进行后续章节的开发时，我们需要**同时运行**后端 `http://127.0.0.1:8000/` 和前端 `http://localhost:8080/` 两个服务器，别搞混了。

## 21.文章列表

本章就正式开始写基于 Vue 3 的前端页面了，具体来说就是编写一个简洁的文章列表页面

### 准备工作

**安装Axios**

虽然现在前后端 Django + Vue 都有了，但还缺一个它们之间通信的手段。Vue 官方推荐的是 [axios](https://github.com/axios/axios) 这个前端库。

命令行进入 `frontend` 目录，安装 axios：

```
> npm install axios
```

### 解决跨域

跨域问题是由于浏览器的同源策略（域名，协议，端口均相同）造成的，是浏览器施加的安全限制。说简单点，Vue 服务器端口（8080）和 Django 服务器端口（8000）不一致，因此无法通过 Javascript 代码请求后端资源。

解决办法有两种。

**第一种方法**是创建 `frontend/vue.config.js` 文件并写入：

```
module.exports = {
    devServer: {
        proxy: {
            '/api': {
                target: `http://127.0.0.1:8000/api`,
                changeOrigin: true,
                pathRewrite: {
                    '^/api': ''
                }
            }
        }
    }
};
```

这个 Vue 的配置文件给前端服务器设置了代理，即将 `/api` 地址的前端请求转发到 8000 端口的后端服务器去，从而规避跨域问题。

**另一种方法**是在后端引入 `django-cors-middleware` 这个库，在后端解决此问题。

> 此方法具体步骤百度很多，就不赘述了。

两种解决方法都可以，本文将选择第一种即前端代理的方法。

### Vue结构

本教程假定读者已经具有了 `Javascript` / `Html` / `Css` 等前端基础知识，因此不会展开讲相关内容。但为了理解 Vue 的基本结构，让我们来看三个重要的文件。

#### index.html

此文件路径位于 `frontend/public/index.html`，内容如下：

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    ...
  </head>
  <body>
    ...
    <div id="app"></div>
    <!-- built files will be auto injected -->
  </body>
</html>
```

这个页面是整个前端工程提供 html 的入口，里面的 `<div id="app">` 是 Vue 初始化应用程序的根容器。

不过在前端工程化的思想中，我们很少会直接去写这类 `html` 文件。

#### main.js

此文件位于 `frontend/src/main.js` ，内容如下：

```javascript
import {createApp} from 'vue'
import App from './App.vue'

createApp(App).mount('#app');
```

它的作用就是把后续你要写的 Vue 组件挂载到刚才那个 `index.html` 中。

如果你有些前端的初始化配置，都可以写到这里。

#### App.vue

此文件位于 `frontend/src/App.vue` ，内容如下：

```vue
<template>
    <img alt="Vue logo" src="./assets/logo.png">
    <HelloWorld msg="Welcome to Your Vue.js App"/>
</template>

<script>
    import HelloWorld from './components/HelloWorld.vue'
    export default {
        name: 'App',
        components: {
            HelloWorld
        }
    }
</script>

<style>
    #app {
        font-family: Avenir, Helvetica, Arial, sans-serif;
        -webkit-font-smoothing: antialiased;
        -moz-osx-font-smoothing: grayscale;
        text-align: center;
        color: #2c3e50;
        margin-top: 60px;
    }
</style>
```

仔细看一下，这个文件似乎就是对应 Vue 的欢迎页面嘛！

Vue 采用组件化的思想，把同一个组件的内容打包到一起。比如这个默认的 `App.vue` 文件，明显 `<template>` 标签就对应传统的 `html` ，`<script>` 标签对应 `javascript` ，`<style>` 标签对应了 `css` 。

`<HelloWorld .../>` 这个就是一个封装好的组件，路径位于 `frontend/src/components/HelloWorld.vue` 。

以上就是 Vue 项目三个重要的文件，而对入门者来说，最重要的就是各种 `.vue` 文件，这就是你最主要的写代码的地方。

> 翻一翻 frontend 中的每个文件，感受 Vue 项目的结构。

### 文章列表

接下来就实际尝试一下编写文章列表页面了，通常这也是博客的首页。

首先把 `App.vue` 中的默认代码都删掉，写入以下代码：

```vue
<!--   frontend/src/App.vue   -->

<template>
    <div v-for="article in info.results" v-bind:key="article.url" id="articles">
        <div class="article-title">
            {{ article.title }}
        </div>
    </div>
</template>

<script>
    import axios from 'axios';

    export default {
        name: 'App',
        data: function () {
            return {
                info: ''
            }
        },
        mounted() {
            axios
                .get('/api/article')
                .then(response => (this.info = response.data))
        }
    }
</script>

<style>
    #articles {
        padding: 10px;
    }

    .article-title {
        font-size: large;
        font-weight: bolder;
        color: black;
        text-decoration: none;
        padding: 5px 0 5px 0;
    }
</style>
```

像前面说的一样，Vue 把同一个组件的 `template` / `script` / `style` 打包到一起。

让我们先从 `<script>` 脚本看起。

#### script

当一个 Vue 实例被创建时，它将 `data` 对象返回的所有属性加入到 Vue 的**响应式系统**中。更神奇的是，当这些属性的值发生改变时，视图将会产生“响应”，即自动更新为新的值。

比方说上面代码的 `data` 中的 `info` 属性在初始化时赋值了一个空字符串。当 Vue 加载完成后调用了生命周期的钩子 `mounted()` 方法，通过 `axios` 向 Django 后端获取到文章列表数据并赋值给 `info` 后，页面中关联的部分也会立即随之更新，而不用你手动去操作页面元素，这就是响应式的好处。

> Axios 自动将请求得到的 Json 数据转换为 JavaScript 对象，所以你可以直接调用接口里的数据了。
>
> 如果写好上述代码，访问 `http://localhost:8080/` 看不到任何内容，请检查后端服务器是否已启动，并查看浏览器控制台确保没有跨域相关的报错。

#### template

如果你之前学过 Django 内置的模板语法，那么 Vue 的模板语法就不难理解。元素块中以 `v` 打头的属性即是 Vue 的模板语法标记。 `v-for` 即循环可迭代元素（`info.results` 对应后端数据的 json 结构。请对照后端接口进行理解。），`v-bind:key` 给定了循环中每个元素的主键，作用是方便 Vue 渲染时对元素进行识别。

注意，很巧的是 Vue 默认同样也用双花括号`{{ }}` 定义它所持有的数据对象。所以这里的双花括号和 Django 模板语法没有任何关系，千万别搞混了。

#### style

这部分纯粹就是 `css` 了，也就是规定了页面各元素的大小、位置、颜色等样式，比较基础就不展开讲了。

顺利的话（别忘了前后端服务器都要启动），现在你的页面应该时这样子：（通过后台添加一些测试文章）

 ![image-20230303233145659](assets/image-20230303233145659.png)

虽然很简陋，但是成功把文章列表数据渲染出来了。

### 优化界面

继续给列表数据添加内容，比如显示后端辛辛苦苦开发的标签和创建时间：

```vue
<!--   frontend/src/App.vue   -->

<template>
    
    ...
    
    <div v-for="...">
        <div>
            <span 
                  v-for="tag in article.tags" 
                  v-bind:key="tag" 
                  class="tag"
            >
                {{ tag }}
            </span>
        </div>

        <div class="article-title">...</div>

        <div>{{ formatted_time(article.created) }}</div>
    </div>

</template>

<script>
    ...
    export default {
        ...
        data: function () {...},
        mounted() {...},    // 注意添加这个逗号！
        methods: {
            formatted_time: function (iso_date_string) {
                const date = new Date(iso_date_string);
                return date.toLocaleDateString()
            }
        }
    }
</script>

<style>
    ...
    
    .tag {
        padding: 2px 5px 2px 5px;
        margin: 5px 5px 5px 0;
        font-family: Georgia, Arial, sans-serif;
        font-size: small;
        background-color: #4e4e4e;
        color: whitesmoke;
        border-radius: 5px;
    }
</style>
```

标签 `tag` 和文章标题类似，用 `v-for` 循环取值即可。

创建时间时 `article.created` 由于需要格式化，则用到点新东西：方法（即methods，注意看 `scripts` 中对其的定义）。方法名为 `formatted_time()` ，功能很简单，即把 iso 日期转换为人类容易理解的日期显示形式。

方法 `methods` 既可以在脚本中直接调用，也可以在模板中通过标签属性或者花括号调用，非常方便。

刷新页面，可以看到标签和日期都成功显示出来了。

> 记得在后台中添加适当的标签数据哦。

### 页眉和页脚

博客网站有页眉和页脚才比较美观，因此继续添加这部分内容：

```vue
<!--   frontend/src/App.vue   -->

<template>

    <div id="header">
        <h1>Welcome to cy’s world.</h1>
        <hr>
    </div>


    <div v-for="..." id="articles">...</div>


    <div id="footer">
        <p>cymoonlight.xyz</p>
    </div>

</template>

<style>
    #app {
        font-family: Georgia, Arial, sans-serif;
        margin-left: 40px;
        margin-right: 40px;
    }

    #header {
        text-align: center;
        margin-top: 20px;
    }

    #footer {
        position: fixed;
        left: 0;
        bottom: 0;
        height: 50px;
        width: 100%;
        background: whitesmoke;
        text-align: center;
        font-weight: bold;
    }
    
    ...
    
</style>
```

没有新知识，唯一需要注意的就是样式中的 `#app` ，它是由 Vue 自动挂载的，因此覆盖了整个页面的元素。

现在你的博客页面是这样子的：

![image-20230303235546318](assets/image-20230303235546318.png)

### 组件化

组件化是 Vue 的核心思想之一。组件可以把网页分解成一个个的小功能，达到代码解耦及复用。

在 `frontend/src/components/` 路径下分别创建 `ArticleList.vue` / `BlogHeader.vue` / `BlogFooter.vue` 三个文件，并且把我们之前在 `App.vue` 中写的代码分别搬运到对应的位置。

三个文件的内容如下（注意 `export` 中的 `name` 有对应的更改）：

ArticleList.vue：

```vue
<template>
    <div v-for="article in info.results" v-bind:key="article.url" id="articles">
        <div>
            <span v-for="tag in article.tags" v-bind:key="tag" class="tag">
                {{ tag }}
            </span>
        </div>

        <div class="article-title">
            {{ article.title }}
        </div>

        <div>{{ formatted_time(article.created) }}</div>
    </div>
</template>

<script>
import axios from 'axios'

export default {
    name: 'ArticleList',
    data: function () {
        return {
            info: ''
        }
    },
    mounted() {
        axios
            .get('/api/article')
            .then(response => (this.info = response.data))
    },
    methods: {
        formatted_time: function (iso_date_string) {
            const date = new Date(iso_date_string);
            return date.toLocaleDateString()
        }
    }
}
</script>

<!-- "scoped" 使样式仅在当前组件生效 -->
<style scoped>
    #articles {
        padding: 10px;
    }

    .article-title {
        font-size: large;
        font-weight: bolder;
        color: black;
        text-decoration: none;
        padding: 5px 0 5px 0;
    }

    .tag {
        padding: 2px 5px 2px 5px;
        margin: 5px 5px 5px 0;
        font-family: Georgia, Arial, sans-serif;
        font-size: small;
        background-color: #4e4e4e;
        color: whitesmoke;
        border-radius: 5px;
    }
</style>
```

BlogHeader.vue：

```vue
<template>
  <div id="header">
    <h1>Welcome to Cy's world.</h1>
    <hr>
  </div>
</template>

<script>
    export default{
        name: 'BlogHeader'
    }
</script>

<!-- "scoped" 使样式仅在当前组件生效 -->
<style scoped>
  #header {
    text-align: center;
    margin-top: 20px;
  }
</style>
```

BlogFooter.vue：

```vue
<template>
    <!-- br 标签给页脚留出位置 -->
    <br><br><br>
    <div id="footer">
        <p>cymoonlight.xyz</p>
        <a href="https://beian.miit.gov.cn/" target="_blank">吉ICP备2023001114号</a>
    </div>
</template>
  
<script>
    export default{
        name: 'BlogFooter'
    }
</script>

<!-- "scoped" 使样式仅在当前组件生效 -->
<style scoped>
  #footer {
    position: fixed;
    left: 0;
    bottom: 0;
    height: 50px;
    width: 100%;
    background: whitesmoke;
    text-align: center;
    font-weight: bold;
  }
</style>
```

搬运完成后，最后将 `App.vue` 修改为如下：

```vue
<template>
  
  <BlogHeader/>

  <ArticleList/>

  <BlogFooter/>

</template>

<script>
  import ArticleList from './components/ArticleList.vue';
  import BlogFooter from './components/BlogFooter.vue';
  import BlogHeader from './components/BlogHeader.vue';

  export default {
    name: 'App',
    components: { BlogHeader, BlogFooter, ArticleList }
  }
</script>

<style>
  #app {
    font-family: Georgia, Arial, sans-serif;
    margin-left: 40px;
    margin-right: 40px;
  }
</style>
```

刷新页面，功能虽然与修改前完全相同，但代码变得更加规整和清爽了。

## 22.文章详情

上一章做好了文章列表，紧接着就是实现**文章详情页面**了。

从列表到详情，首当其冲的问题就是页面如何跳转。

传统模式的跳转是由 Django 后端分配路由。不过本教程既然采用了前后端分离的模式，那就打算抛弃后端路由，采用**前端路由**的方式来实现页面跳转。

### 准备工作

首先安装 Vue 的官方前端路由库 **vue-router**：

```
> npm install vue-router@4

...

+ vue-router@4.0.2
added 1 package in 9.143s
```

> 笔者这里安装到的 4.1.6 版本。

因为 vue-router 会用到文章的 id 作为动态地址，所以对 **Django 后端**做一点小更改：

```
# article/serializers.py

class ArticleBaseSerializer(serializers.HyperlinkedModelSerializer):
    id = serializers.IntegerField(read_only=True)
    
    ...
```

简单的把文章的 id 值增加到接口数据中。

### Router

接下来就正式开始配置前端路由了。

首先把 vue-router 加载到 Vue 实例中：

```
// frontend/src/main.js

...

import router from './router'

createApp(App).use(router).mount('#app');
```

> 和 **Vue 2** 不同的是，挂载路由实例时 **Vue 3** 采用函数式的写法，变得更加美观了。

由于后续页面会越来越多，为了避免 `App.vue` 越发臃肿，因此必须优化文件结构。

新建 `frontend/src/views/` 目录，用来存放现在及将来所有的页面文件。在此目录新建 `Home.vue` 文件，把之前的首页代码稍加修改搬运过来：

```vue
<!--  frontend/src/views/Home.vue  -->

<template>

    <BlogHeader/>

    <ArticleList/>

    <BlogFooter/>

</template>

<script>
    import BlogHeader from '@/components/BlogHeader.vue'
    import BlogFooter from '@/components/BlogFooter.vue'
    import ArticleList from '@/components/ArticleList.vue'

    export default {
        name: 'Home',
        components: {BlogHeader, BlogFooter, ArticleList}
    }
</script>
```

新增文章详情页面：

```vue
<!--  frontend/src/views/ArticleDetail.vue  -->

<template>

    <BlogHeader/>

    <!--  暂时留空  -->

    <BlogFooter/>

</template>

<script>
    import BlogHeader from '@/components/BlogHeader.vue'
    import BlogFooter from '@/components/BlogFooter.vue'

    export default {
        name: 'ArticleDetail',
        components: {BlogHeader, BlogFooter}
    }
</script>
```

页面暂时只有个壳子，一会儿来添加实际功能。

修改 `App.vue`：

```vue
<!--  frontend/src/App.vue  -->

<template>
    <router-view/>
</template>

<script>
    export default {
        name: 'App'
    }
</script>

<style>
    ...
</style>
```

`App.vue` 文件中大部分内容都搬走了，只剩一个新增的 `<router-view>` 标签，它就是各路径所代表的页面的实际渲染位置。比如你现在在 Home 页面，那么 `<router-view>` 则渲染的是 Home 中的内容。

> 一套组合拳，App.vue 看起来干净多了。

这些都搞好了之后，新建 `frontend/src/router/index.js` 文件用于存放路由相关的文件，写入：

```javascript
// frontend/src/router/index.js

import {createWebHistory, createRouter} from "vue-router";
import Home from "@/views/Home.vue";
import ArticleDetail from "@/views/ArticleDetail.vue";

const routes = [
    {
        path: "/",
        name: "Home",
        component: Home,
    },
    {
        path: "/article/:id",
        name: "ArticleDetail",
        component: ArticleDetail
    }
];

const router = createRouter({
    history: createWebHistory(),
    routes,
});

export default router;
```

- 列表 `routes` 定义了所有需要挂载到路由中的路径，成员为**路径 url** 、**路径名**和**路径的 vue 对象**。详情页面的动态路由采用冒号 `:id` 的形式来定义。
- 接着就用 `createRouter()` 创建 router。参数里的 `history` 定义具体的路由形式，`createWebHashHistory()` 为哈希模式（具体路径在 # 符号后面）；`createWebHistory()` 为 HTML5 模式（路径中没有丑陋的 # 符号），此为**推荐模式**，但是**部署时需要额外的配置**。

> 各模式的详细介绍看[文档](https://next.router.vuejs.org/guide/essentials/history-mode.html)。

搞定这些后，修改首页的组件代码：

```vue
<!--  frontend/src/components/ArticleList.vue  -->

<template>
    <div v-for="...">
        
        <!--<div class="article-title">-->
        <!--{{ article.title }}-->
        <!--</div>-->

        <router-link
                :to="{ name: 'ArticleDetail', params: { id: article.id }}"
                class="article-title"
        >
            {{ article.title }}
        </router-link>

        ...
    </div>
</template>

...
```

调用 vue-router 不再需要常规的 `<a>` 标签了，而是 `<router-link>` 。

`:to` 属性指定了跳转位置，注意看**动态参数 id** 是如何传递的。

在 Vue 中，属性前面的冒号 `:` 表示此属性被”绑定“了。”绑定“的对象可以是某个动态的参数（比如这里的 id 值），也可以是 Vue 所管理的 data，也可以是 methods。总之，看到冒号就要明白这个属性后面跟着个变量或者表达式，没有冒号就是普通的字符串。冒号 `:` 实际上是 `v-bind:` 的缩写。

> 有一个小问题是由于 router 内部机制，之前给 `class="article-title"` 写的 padding 样式会失效。解决方式是将其包裹在一个 div 元素中，在此 div 上重新定义 padding。想了解做法的见 Github 仓库中的源码。

Router 骨架就搭建完毕了。此时点击首页的文章标题链接后，应该就顺利跳转到一个只有页眉页脚的详情页面了。

> 注意查看浏览器控制栏，有任何报错都表明代码不正确。

### 编写详情页面

接下来就正式写详情页面了。

代码量稍稍有点多，一并贴出来：

```vue
<!--  frontend/src/views/ArticleDetail.vue  -->

<template>

    <BlogHeader/>

    <div v-if="article !== null" class="grid-container">
        <div>
            <h1 id="title">{{ article.title }}</h1>
            <p id="subtitle">
                本文由 {{ article.author.username }} 发布于 {{ formatted_time(article.created) }}
            </p>
            <div v-html="article.body_html" class="article-body"></div>
        </div>
        <div>
            <h3>目录</h3>
            <div v-html="article.toc_html" class="toc"></div>
        </div>
    </div>

    <BlogFooter/>

</template>

<script>
    import BlogHeader from '@/components/BlogHeader.vue'
    import BlogFooter from '@/components/BlogFooter.vue'

    import axios from 'axios';


    export default {
        name: 'ArticleDetail',
        components: {BlogHeader, BlogFooter},
        data: function () {
            return {
                article: null
            }
        },
        mounted() {
            axios
                .get('/api/article/' + this.$route.params.id)
                .then(response => (this.article = response.data))
        },
        methods: {
            formatted_time: function (iso_date_string) {
                const date = new Date(iso_date_string);
                return date.toLocaleDateString()
            }
        }
    }
</script>

<style scoped>
    .grid-container {
        display: grid;
        grid-template-columns: 3fr 1fr;
    }


    #title {
        text-align: center;
        font-size: x-large;
    }

    #subtitle {
        text-align: center;
        color: gray;
        font-size: small;
    }

</style>

<style>
    .article-body p img {
        max-width: 100%;
        border-radius: 50px;
        box-shadow: gray 0 0 20px;
    }

    .toc ul {
        list-style-type: none;
    }

    .toc a {
        color: gray;
    }
</style>
```

先看**模板**部分：

- 在渲染文章前，逻辑控制语句 `v-if` 先确认数据是否存在，避免出现**潜在的**调用数据不存在的 bug。
- 由于 `body_html` 、`toc_html` 都是后端渲染好的 markdown 文本，需要将其直接转换为 HTML ，所以需要用 `v-html` 标注。

再看脚本：

- 通过 `$route.params.id` 可以获得路由中的动态参数，以此拼接为接口向后端请求数据。

最后看**样式**：

- `.grid-container` 简单的给文章内容、目录划分了网格区域。
- `<style>` 标签可以有多个，满足“分块强迫症患者”的需求。这里分两个的原因是文章内容、目录都是从原始 HTML 渲染的，不在 `scoped` 的管理范围内。

大致就这些新知识点了，理解起来似乎也不困难。





































































































