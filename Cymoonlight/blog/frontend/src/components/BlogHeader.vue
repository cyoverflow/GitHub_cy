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