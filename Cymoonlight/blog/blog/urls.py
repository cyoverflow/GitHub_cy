from django.contrib import admin
from django.urls import path, include

from rest_framework.routers import DefaultRouter
from article import views

from django.conf import settings
from django.conf.urls.static import static

# 这里直接导入 views会冲突
from comment.views import CommentViewSet

# 添加 Token 的获取和刷新地址
from rest_framework_simplejwt.views import(
    TokenObtainPairView,
    TokenRefreshView,
)
# 用户管理
from user_info.views import UserViewSet

router = DefaultRouter()
router.register(r'article', views.ArticleViewSet)
router.register(r'category', views.CategoryViewSet)
router.register(r'tag', views.TagViewSet)
router.register(r'avatar', views.AvatarViewSet)
router.register(r'comment', CommentViewSet)
router.register(r'user', UserViewSet)

urlpatterns = [
    path('admin/', admin.site.urls),
    # DRF登录视图
    path('api-auth/', include('rest_framework.urls')),

    # drf自动注册路由
    path('api/', include(router.urls)),
    # # article
    # path('api/article/', include('article.urls', namespace='article')),

    # JWT
    path('api/token/', TokenObtainPairView.as_view(), name='token_obtain_pair'),
    path('api/token/refresh/', TokenRefreshView.as_view(), name='token_refresh'),
]

# 注册媒体文件的路由
if settings.DEBUG:
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
