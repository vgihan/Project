package kr.or.connect.guestbook.dao;

import javax.sql.DataSource;

import org.springframework.jdbc.core.namedparam.BeanPropertySqlParameterSource;
import org.springframework.jdbc.core.namedparam.SqlParameterSource;
import org.springframework.jdbc.core.simple.SimpleJdbcInsert;
import org.springframework.stereotype.Repository;

import kr.or.connect.guestbook.dto.Log;

@Repository
public class LogDao {
	private SimpleJdbcInsert insert;
	
	public LogDao(DataSource dataSource) {
		this.insert = new SimpleJdbcInsert(dataSource)
				.withTableName("Log")
				.usingGeneratedKeyColumns("id");
	}
	
	public Long insert(Log log) {
		SqlParameterSource params = new BeanPropertySqlParameterSource(log);
		return insert.executeAndReturnKey(params).longValue();
	}
}
