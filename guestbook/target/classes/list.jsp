<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
<head>
<meta charset="EUC-KR">
<title>방명록</title>
</head>
<body>
	<h1>방명록</h1>
방명록 전체 수 : ${count}<br><br>
<c:forEach var="guestbook" items="${list}">
${guestbook.id}<br>
${guestbook.name}<br>
${guestbook.content}<br>
${guestbook.regdate}<br>
</c:forEach>

<c:forEach items="${pageStartList}" var="start" varStatus="status">
	<a href="list?start=${start}">${status.count} </a>&nbsp;
</c:forEach>

<form action="write" method="post">
	name : <input type="text" name="name"><br>
	<textarea name="content"></textarea><br>
	<input type="submit" value="등록">
</form>
</body>
</html>