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





















