package org.edwith.webbe.cardmanager.dao;

import org.edwith.webbe.cardmanager.dto.BusinessCard;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class BusinessCardManagerDao {
	private static String url = "jdbc:mysql://localhost:3306/connectdb?serverTimezone=UTC";
	private static String user = "connectuser";
	private static String password = "connect123!@#";
    public List<BusinessCard> searchBusinessCard(String keyword){
    	List<BusinessCard> list = new ArrayList<>();
    	
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
		} catch (Exception e) {
			e.printStackTrace();
		}
		
    	String sql = "SELECT * FROM BusinessCard " +
    			     "WHERE name LIKE ?";
    	
    	try (Connection connection = DriverManager.getConnection(url, user, password);
    		 PreparedStatement statement = connection.prepareStatement(sql)){
			
    		statement.setString(1, "%"+keyword+"%");
    		
    		try(ResultSet resultSet = statement.executeQuery()){
				while(resultSet.next()) {
					String name = resultSet.getString("name");
					String phone = resultSet.getString("phone");
					String companyName = resultSet.getString("companyName");
					BusinessCard card = new BusinessCard(name, phone, companyName);
					list.add(card);
				}
			} catch(Exception e) {
				e.printStackTrace();
			}
    		
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    	
    	return list;
    }

    public int addBusinessCard(BusinessCard businessCard){
    	int insertCount = 0;
    	
    	try {
			Class.forName("com.mysql.cj.jdbc.Driver");
		} catch (Exception e) {
			e.printStackTrace();
		}
    	
    	String sql = "INSERT INTO BusinessCard(name, phone, companyName) " +
    				 "VALUES(?, ?, ?)";
    	
    	try(Connection connection = DriverManager.getConnection(url, user, password);
    		PreparedStatement statement = connection.prepareStatement(sql)){
    		
    		String name = businessCard.getName();
    		String phone = businessCard.getPhone();
    		String companyName = businessCard.getCompanyName();
    		
    		statement.setString(1, name);
    		statement.setString(2, phone);
    		statement.setString(3, companyName);
    		
    		insertCount = statement.executeUpdate();
    		
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    	
    	return insertCount;
    }
}
