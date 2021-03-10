package kr.or.connect.guestbook.dao;

import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.sql.DataSource;

import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.core.namedparam.BeanPropertySqlParameterSource;
import org.springframework.jdbc.core.namedparam.NamedParameterJdbcTemplate;
import org.springframework.jdbc.core.namedparam.SqlParameterSource;
import org.springframework.jdbc.core.simple.SimpleJdbcInsert;
import org.springframework.stereotype.Repository;

import kr.or.connect.guestbook.dto.Guestbook;
import static kr.or.connect.guestbook.dao.GuestbookDaoSqls.SELECT_ALL;
import static kr.or.connect.guestbook.dao.GuestbookDaoSqls.DELETE_ONE;
import static kr.or.connect.guestbook.dao.GuestbookDaoSqls.UPDATE_ONE;
import static kr.or.connect.guestbook.dao.GuestbookDaoSqls.GET_COUNT;

@Repository
public class GuestbookDao {
	private NamedParameterJdbcTemplate jdbc;
	private SimpleJdbcInsert insert;
	private RowMapper<Guestbook> rowMapper = BeanPropertyRowMapper.newInstance(Guestbook.class);
	
	public GuestbookDao(DataSource dataSource) {
		this.jdbc = new NamedParameterJdbcTemplate(dataSource);
		this.insert = new SimpleJdbcInsert(dataSource)
				.withTableName("guestbook")
				.usingGeneratedKeyColumns("id");
	}
	
	public List<Guestbook> selectAll(Integer start, Integer limit){
		Map<String, Integer> params = new HashMap<>();
		params.put("start", start);
		params.put("limit", limit);
		return jdbc.query(SELECT_ALL, params, rowMapper);
	}
	
	public Long insert(Guestbook guestbook) {
		SqlParameterSource params = new BeanPropertySqlParameterSource(guestbook);
		return insert.executeAndReturnKey(params).longValue();
	}
	
	public int delete(Long id) {
		Map<String, Long> params = new HashMap<>();
		params.put("id", id);
		return jdbc.update(DELETE_ONE, params);
	}
	
	public int update(Guestbook guestbook) {
		SqlParameterSource params = new BeanPropertySqlParameterSource(guestbook);
		return jdbc.update(UPDATE_ONE, params);
	}
	
	public int selectCount() {
		return jdbc.queryForObject(GET_COUNT, Collections.emptyMap(), Integer.class);
	}
}
