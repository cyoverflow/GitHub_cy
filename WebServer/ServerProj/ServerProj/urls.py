"""ServerProj URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.conf.urls import url
# from django.urls import include, re_path
from django.urls import path
from . import views, testdb, search, search2

urlpatterns = [
    # url(r'^$', views.hello),
    # path('test/', views.hello)
    path('', views.homepage),               # 默认主页
    # re_path(r'^[h|H]ello/$', views.helloPage)
    path('test1/', views.hello),            # 模板测试
    path('var_test/', views.varTest),       # 变量测试
    path('list_test/', views.listTest),     # 列表测试
    path('dict_test/', views.dictTest),     # 字典测试
    path('def_test/', views.defTest),       # default过滤器测试
    path('length_test/', views.lengthtest), # length过滤器测试
    path('fsf_test/', views.fSizeFmttest),  # filesizeformat过滤器测试
    path('date_test/', views.datetest),     # date过滤器测试
    path('trun_test/', views.truncatetest), # truncatechars过滤器测试
    path('safe_test/', views.safetest),     # safe过滤器测试
    path('ifelse_test/', views.ifelsetest), # if/else标签测试
    path('for_test/', views.fortest),       # for标签测试
    path('static_test/', views.imgtest),    # static文件导入图片测试
    path('extend_test/', views.extendtest), # 模板继承测试

    path('adddb/', testdb.dbaddTest),       # 数据库添加
    path('getdb/', testdb.dbgetTest),       # 数据库获取
    path('updatedb/', testdb.dbupdateTest), # 数据库更新
    path('deletedb/', testdb.dbdelTest),    # 数据库删除

    url(r'^search-form/$', search.search_form),     # GET方法
    url(r'^search/$', search.search),          

    url(r'^search-post/$', search2.search_post),    # POST方法

    url(r'^get-test/$', views.gettest),             # GET测试
    url(r'^body-test/$', views.bodytest),           # body测试
    url(r'^path-test/$', views.pathtest),           # path测试
    url(r'^method-test/$', views.methodtest),       # method测试

    url(r'^res-test/$', views.httpresTest),         # HttpResponse响应测试
    url(r'^redirect-test/$', views.redirectTest),   # redirect响应测试
]
