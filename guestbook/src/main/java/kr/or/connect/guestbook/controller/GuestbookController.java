package kr.or.connect.guestbook.controller;

import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

import kr.or.connect.guestbook.dto.Guestbook;
import kr.or.connect.guestbook.service.GuestbookService;

@Controller
public class GuestbookController {
	private static int pageStart = 0;
	
	@Autowired
	GuestbookService guestbookService;
	
	@GetMapping(path = "list")
	public String list(@RequestParam(name = "start", required = false, defaultValue = "0") Integer start, ModelMap modelMap) {
		List<Guestbook> list = guestbookService.getGuestbooks(start);
		
		pageStart = start;
		
		int count = guestbookService.getCount();
		int pageCount = count / GuestbookService.LIMIT;
		if(count % GuestbookService.LIMIT != 0) 
			pageCount++;
		
		List<Integer> pageStartList = new ArrayList<>();
		for(int i=0; i<pageCount; i++) {
			pageStartList.add(i*GuestbookService.LIMIT);
		}

		modelMap.addAttribute("pageStartList", pageStartList);
		modelMap.addAttribute("count", count);
		modelMap.addAttribute("list", list);
		
		return "list";
	}
	
	@PostMapping(path = "write")
	public String write(@ModelAttribute Guestbook guestbook, HttpServletRequest request) {
		String clientIp = request.getRemoteAddr();
		System.out.println("clientIP: "+clientIp);
		guestbookService.addGuestbook(guestbook, clientIp);
		
		return "redirect:list?start=" + pageStart;
	}
}
