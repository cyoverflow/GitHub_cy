from rest_framework import serializers
from article.models import Article

# 父类变成了 ModelSerializer
class ArticleListSerializer(serializers.ModelSerializer):
    class Meta:
        model = Article
        fields = [
            'id',
            'title',
            'body',
            'created',
        ]

class ArticleDetailSerializer(serializers.ModelSerializer):
    class Meta:
        model = Article
        fields = '__all__'