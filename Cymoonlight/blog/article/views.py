from rest_framework.decorators import api_view
from rest_framework.response import Response
from rest_framework import status
from article.models import Article
from article.serializers import ArticleListSerializer

# 基于类的视图
from rest_framework.views import APIView
from django.http import Http404
from article.serializers import ArticleDetailSerializer

# 通用视图
from rest_framework import mixins
from rest_framework import generics

# 权限控制
# from rest_framework.permissions import IsAdminUser
from article.permissions import IsAdminUserOrReadOnly

"""最简版:文章列表接口"""
class ArticleList(generics.ListCreateAPIView):
    queryset = Article.objects.all()
    serializer_class = ArticleListSerializer
    # 权限控制
    permission_classes = [IsAdminUserOrReadOnly]

"""最初版:文章列表接口"""
# @api_view(['GET', 'POST'])
# def article_list(request):
#     if request.method == 'GET':
#         articles = Article.objects.all()
#         serializer = ArticleListSerializer(articles, many=True)
#         return Response(serializer.data)
    
#     elif request.method == 'POST':
#         serializer = ArticleListSerializer(data=request.data)
#         if serializer.is_valid():
#             serializer.save()
#             return Response(serializer.data, status=status.HTTP_201_CREATED)
#         return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
    
"""最简版:文章详情接口"""    
class ArticleDetail(generics.RetrieveUpdateDestroyAPIView):
    queryset = Article.objects.all()
    serializer_class = ArticleDetailSerializer
    # 权限控制
    permission_classes = [IsAdminUserOrReadOnly]

"""简洁版:使用mixins"""
# class ArticleDetail(mixins.RetrieveModelMixin,
#                     mixins.UpdateModelMixin,
#                     mixins.DestroyModelMixin,
#                     generics.GenericAPIView):
#     """文章详情视图"""
#     queryset = Article.objects.all()
#     serializer_class = ArticleDetailSerializer

#     def get(self, request, *args, **kwargs):
#         return self.retrieve(request, *args, **kwargs)
    
#     def put(self, request, *args, **kwargs):
#         return self.update(request, *args, **kwargs)
    
#     def delete(self, request, *args, **kwargs):
#         return self.delete(request, *args, **kwargs)
"""最初版:文章详情接口"""    
# class ArticleDetail(APIView):
#     """文章详情视图"""

#     def get_object(self, pk):
#         """获取单个文章对象"""
#         try:
#             # pk即主键(primary key),默认状态下是id
#             return Article.objects.get(pk=pk)
#         except:
#             raise Http404
        
#     def get(self, request, pk):
#         article = self.get_object(pk)
#         serializer = ArticleDetailSerializer(article)
#         # 返回Json数据
#         return Response(serializer.data)
    
#     def put(self, request, pk):
#         article = self.get_object(pk)
#         serializer = ArticleDetailSerializer(article, data=request.data)
#         # 验证提交的数据是否合法
#         # 不合法则返回404
#         if serializer.is_valid():
#             # 序列化器将持有的数据反序列化后,
#             # 保存到数据库中
#             serializer.save()
#             return Response(serializer.data)
#         return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
    
#     def delete(self, request, pk):
#         article = self.get_object(pk)
#         article.delete()
#         # 删除成功后返回204
#         return Response(status=status.HTTP_204_NO_CONTENT)