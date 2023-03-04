<template>
  
    <BlogHeader/>
  
    <ArticleList/>
  
    <BlogFooter/>
  
  </template>
  
  <script>
    import ArticleList from '@/components/ArticleList.vue'
    import BlogFooter from '@/components/BlogFooter.vue'
    import BlogHeader from '@/components/BlogHeader.vue'
    // eslint-disable-next-line vue/multi-word-component-names
    export default {
    // eslint-disable-next-line vue/multi-word-component-names
      name: 'Home',
      components: { BlogHeader, BlogFooter, ArticleList }
    }
  </script>