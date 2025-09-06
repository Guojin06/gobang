#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import re
from datetime import datetime

def clean_html(text):
    """清理HTML标签，保留文本内容"""
    if not text:
        return ""
    # 移除HTML标签，保留内容
    clean = re.sub('<[^<]+?>', '', text)
    # 处理HTML实体
    clean = clean.replace('&lt;', '<').replace('&gt;', '>')
    return clean.strip()

def format_date(date_str):
    """格式化日期"""
    if not date_str:
        return ""
    try:
        dt = datetime.fromisoformat(date_str.replace('Z', '+00:00'))
        return dt.strftime('%Y.%m')
    except:
        return date_str

def convert_to_markdown(resume_data):
    """将简历JSON转换为Markdown格式"""
    md_content = []
    
    # 基本信息
    basic = resume_data.get('basic', {})
    md_content.append(f"# {basic.get('name', '')}")
    md_content.append(f"## {basic.get('title', '')}")
    md_content.append("")
    
    # 联系信息
    contact_info = []
    if basic.get('email'):
        contact_info.append(f"📧 {basic['email']}")
    if basic.get('phone'):
        contact_info.append(f"📱 {basic['phone']}")
    if basic.get('customFields'):
        for field in basic['customFields']:
            if field.get('visible', True):
                contact_info.append(f"{field.get('label', '')}: {field.get('value', '')}")
    
    if contact_info:
        md_content.append(" | ".join(contact_info))
        md_content.append("")
    
    # 教育经历
    education = resume_data.get('education', [])
    if education:
        md_content.append("## 🎓 教育经历")
        md_content.append("")
        for edu in education:
            if edu.get('visible', True):
                start_date = format_date(edu.get('startDate'))
                end_date = format_date(edu.get('endDate'))
                date_range = f"{start_date} - {end_date}" if start_date and end_date else ""
                
                md_content.append(f"### {edu.get('school', '')} | {edu.get('major', '')} | {edu.get('degree', '')}")
                if date_range:
                    md_content.append(f"**时间**: {date_range}")
                if edu.get('gpa'):
                    md_content.append(f"**GPA**: {edu['gpa']}")
                if edu.get('description'):
                    md_content.append(clean_html(edu['description']))
                md_content.append("")
    
    # 专业技能
    skills = resume_data.get('skillContent', '')
    if skills:
        md_content.append("## ⚡ 专业技能")
        md_content.append("")
        md_content.append(clean_html(skills))
        md_content.append("")
    
    # 工作经验
    experience = resume_data.get('experience', [])
    if experience:
        md_content.append("## 💼 工作经验")
        md_content.append("")
        for exp in experience:
            if exp.get('visible', True):
                md_content.append(f"### {exp.get('company', '')} | {exp.get('position', '')}")
                if exp.get('date'):
                    md_content.append(f"**时间**: {exp['date']}")
                if exp.get('details'):
                    md_content.append(clean_html(exp['details']))
                md_content.append("")
    
    # 项目经历
    projects = resume_data.get('projects', [])
    if projects:
        md_content.append("## 🚀 项目经历")
        md_content.append("")
        for project in projects:
            if project.get('visible', True):
                md_content.append(f"### {project.get('name', '')} | {project.get('role', '')}")
                if project.get('date'):
                    md_content.append(f"**时间**: {project['date']}")
                if project.get('description'):
                    md_content.append(clean_html(project['description']))
                md_content.append("")
    
    # 自定义部分
    custom_data = resume_data.get('customData', {})
    menu_sections = resume_data.get('menuSections', [])
    
    for section in menu_sections:
        if section.get('enabled', False) and section['id'].startswith('custom-'):
            section_data = custom_data.get(section['id'], [])
            if section_data:
                md_content.append(f"## {section.get('icon', '➕')} {section.get('title', '')}")
                md_content.append("")
                for item in section_data:
                    if item.get('visible', True):
                        if item.get('title'):
                            md_content.append(f"### {item['title']}")
                        if item.get('subtitle'):
                            md_content.append(f"**{item['subtitle']}**")
                        if item.get('dateRange'):
                            md_content.append(f"**时间**: {item['dateRange']}")
                        if item.get('description'):
                            md_content.append(clean_html(item['description']))
                        md_content.append("")
    
    return "\n".join(md_content)

def convert_to_html(resume_data):
    """将简历JSON转换为HTML格式"""
    html_content = []
    
    # HTML头部
    html_content.append("""<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>个人简历</title>
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif; line-height: 1.6; max-width: 800px; margin: 0 auto; padding: 20px; color: #333; }
        h1 { color: #1A1A1A; margin-bottom: 5px; }
        h2 { color: #1A1A1A; border-bottom: 2px solid #1A1A1A; padding-bottom: 5px; margin-top: 30px; }
        h3 { color: #444; margin-top: 25px; margin-bottom: 10px; }
        .contact-info { color: #666; margin-bottom: 30px; }
        .date-range { color: #888; font-weight: bold; }
        ul { padding-left: 20px; }
        li { margin-bottom: 5px; }
        .section { margin-bottom: 30px; }
    </style>
</head>
<body>""")
    
    # 基本信息
    basic = resume_data.get('basic', {})
    html_content.append(f"<h1>{basic.get('name', '')}</h1>")
    html_content.append(f"<h2 style='border:none; margin-top:0;'>{basic.get('title', '')}</h2>")
    
    # 联系信息
    contact_info = []
    if basic.get('email'):
        contact_info.append(f"📧 {basic['email']}")
    if basic.get('phone'):
        contact_info.append(f"📱 {basic['phone']}")
    if basic.get('customFields'):
        for field in basic['customFields']:
            if field.get('visible', True):
                contact_info.append(f"{field.get('label', '')}: {field.get('value', '')}")
    
    if contact_info:
        html_content.append(f"<div class='contact-info'>{' | '.join(contact_info)}</div>")
    
    # 教育经历
    education = resume_data.get('education', [])
    if education:
        html_content.append("<h2>🎓 教育经历</h2>")
        for edu in education:
            if edu.get('visible', True):
                start_date = format_date(edu.get('startDate'))
                end_date = format_date(edu.get('endDate'))
                date_range = f"{start_date} - {end_date}" if start_date and end_date else ""
                
                html_content.append(f"<h3>{edu.get('school', '')} | {edu.get('major', '')} | {edu.get('degree', '')}</h3>")
                if date_range:
                    html_content.append(f"<div class='date-range'>时间: {date_range}</div>")
                if edu.get('gpa'):
                    html_content.append(f"<div><strong>GPA</strong>: {edu['gpa']}</div>")
                if edu.get('description'):
                    html_content.append(f"<div>{edu['description']}</div>")
    
    # 专业技能
    skills = resume_data.get('skillContent', '')
    if skills:
        html_content.append("<h2>⚡ 专业技能</h2>")
        html_content.append(f"<div>{skills}</div>")
    
    # 工作经验
    experience = resume_data.get('experience', [])
    if experience:
        html_content.append("<h2>💼 工作经验</h2>")
        for exp in experience:
            if exp.get('visible', True):
                html_content.append(f"<h3>{exp.get('company', '')} | {exp.get('position', '')}</h3>")
                if exp.get('date'):
                    html_content.append(f"<div class='date-range'>时间: {exp['date']}</div>")
                if exp.get('details'):
                    html_content.append(f"<div>{exp['details']}</div>")
    
    # 项目经历
    projects = resume_data.get('projects', [])
    if projects:
        html_content.append("<h2>🚀 项目经历</h2>")
        for project in projects:
            if project.get('visible', True):
                html_content.append(f"<h3>{project.get('name', '')} | {project.get('role', '')}</h3>")
                if project.get('date'):
                    html_content.append(f"<div class='date-range'>时间: {project['date']}</div>")
                if project.get('description'):
                    html_content.append(f"<div>{project['description']}</div>")
    
    # 自定义部分
    custom_data = resume_data.get('customData', {})
    menu_sections = resume_data.get('menuSections', [])
    
    for section in menu_sections:
        if section.get('enabled', False) and section['id'].startswith('custom-'):
            section_data = custom_data.get(section['id'], [])
            if section_data:
                html_content.append(f"<h2>{section.get('icon', '➕')} {section.get('title', '')}</h2>")
                for item in section_data:
                    if item.get('visible', True):
                        if item.get('title'):
                            html_content.append(f"<h3>{item['title']}</h3>")
                        if item.get('subtitle'):
                            html_content.append(f"<div><strong>{item['subtitle']}</strong></div>")
                        if item.get('dateRange'):
                            html_content.append(f"<div class='date-range'>时间: {item['dateRange']}</div>")
                        if item.get('description'):
                            html_content.append(f"<div>{item['description']}</div>")
    
    html_content.append("</body></html>")
    return "\n".join(html_content)

# 使用示例
if __name__ == "__main__":
    # 这里放入您的JSON数据
    json_data = """您的JSON数据"""
    
    try:
        resume_data = json.loads(json_data)
        
        # 转换为Markdown
        markdown_content = convert_to_markdown(resume_data)
        with open("resume.md", "w", encoding="utf-8") as f:
            f.write(markdown_content)
        print("已生成 resume.md")
        
        # 转换为HTML
        html_content = convert_to_html(resume_data)
        with open("resume.html", "w", encoding="utf-8") as f:
            f.write(html_content)
        print("已生成 resume.html")
        
    except json.JSONDecodeError as e:
        print(f"JSON解析错误: {e}")
    except Exception as e:
        print(f"转换错误: {e}")
