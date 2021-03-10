package kr.or.connect.guestbook.dao;

public class GuestbookDaoSqls {
	public static String SELECT_ALL = "SELECT id, name, content, regdate FROM guestbook ORDER BY id DESC LIMIT :start , :limit";
	public static String DELETE_ONE = "DELETE FROM guestbook WHERE id = :id";
	public static String UPDATE_ONE = "UPDATE guestbook SET name = :name, content = :content, regdate = :regdate WHERE id = :id";
	public static String GET_COUNT = "SELECT count(*) FROM guestbook";
}
