from rest_framework import serializers
from article.models import Article
from article.models import Category
from article.models import Tag
from article.models import Avatar
from user_info.serializers import UserDescSerializer

class AvatarSerializer(serializers.ModelSerializer):
    url = serializers.HyperlinkedIdentityField(view_name='avatar-detail')

    class Meta:
        model = Avatar
        fields = '__all__'

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

class CategorySerializer(serializers.ModelSerializer):
    """分类的序列化器"""
    url = serializers.HyperlinkedIdentityField(view_name='category-detail')

    class Meta:
        model = Category
        fields = '__all__'
        read_only_fields = ['created']

# 将已有的 ArticleSerializer 里的东西全部挪到这个 ArticleBaseSerializer 里来
# 除了 Meta 类保留
class ArticleBaseSerializer(serializers.HyperlinkedModelSerializer):
    """文章序列化器"""
    author = UserDescSerializer(read_only=True)
    # tag 字段
    tags = serializers.SlugRelatedField(
        queryset=Tag.objects.all(),
        many=True,
        required=False,
        slug_field='text'
    )
    # 图片字段
    avatar = AvatarSerializer(read_only=True)
    avatar_id = serializers.IntegerField(
        write_only=True,
        allow_null=True,
        required=False
    )

    # 覆写方法,如果输入的标签不存在则创建它
    def to_internal_value(self, data):
        tags_data = data.get('tags')

        if isinstance(tags_data, list):
            for text in tags_data:
                if not Tag.objects.filter(text=text).exists():
                    Tag.objects.create(text=text)

        return super().to_internal_value(data)
    
    # Category 的嵌套序列化字段
    category = CategorySerializer(read_only=True)
    # Category 的id字段，用于创建/更新 category 外键
    category_id = serializers.IntegerField(write_only=True, allow_null=True, required=False)

    # # category_id 字段的验证器
    # def validate_category_id(self, value):
    #     if not Category.objects.filter(id=value).exists() and value is not None:
    #         raise serializers.ValidationError("Category with id {} not exists.".format(value))
    #     return value
    
    # # 验证图片ID是否存在
    # # 不存在则返回验证错误
    # def validate_avatar_id(self, value):
    #     if not Avatar.objects.filter(id=value).exists() and value is not None:
    #         raise serializers.ValidationError("Avatar with id {} not exists.".format(value))
        
    #     return value

    # 将分类和标题图的验证方法抽象出来
    # 自定义错误信息
    default_error_messages = {
        'incorrect_avatar_id': 'Avatar with id {value} not exists.',
        'incorrect_category_id': 'Category with id {value} not exists.',
        'default': 'No more message here...'
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

# 保留Meta类
# 将父类改为 ArticleBaseSerializer
class ArticleSerializer(ArticleBaseSerializer):
    class Meta:
        model = Article
        fields = '__all__'
        extra_kwargs = {'body':{'write_only':True}}

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


# # 父类变成了 ModelSerializer
# class ArticleListSerializer(serializers.ModelSerializer):
#     # 增加超链接字段
#     url = serializers.HyperlinkedIdentityField(view_name="article:detail")

#     # read_only 参数设置为只读
#     author = UserDescSerializer(read_only=True)
    
#     class Meta:
#         model = Article
#         fields = [
#             # 有了url id就不需要了
#             #'id',
#             'url',
#             'title',
#             'created',
#             'author',
#         ]
#         # 嵌套序列化器已经设置了只读，所以不需要以下的设置
#         # 将作者字段设置为只读
#         # read_only_fields = ['author']

# class ArticleDetailSerializer(serializers.ModelSerializer):
#     class Meta:
#         model = Article
#         fields = '__all__'