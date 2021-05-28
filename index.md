---
layout: page
title: 【习题解答】计算机教育中缺失的一课
---


以下是 [计算机教育中缺失的一课](https://missing-semester-cn.github.io/) 配套习题解答及学习笔记：

<ul>
{% assign lectures = site['2020'] | sort: 'date' %}
{% for lecture in lectures %}
    {% if lecture.phony != true %}
        <li>
            <a href="{{ lecture.url }}">{{ lecture.title }}</a>

        </li>
    {% endif %}
{% endfor %}
</ul>

讲座视频可以在 [
YouTube](https://www.youtube.com/playlist?list=PLyzOVJj3bHQuloKGG59rS43e29ro7I57J)上找到。


---

<div class="small center">
<p><a href="https://github.com/missing-semester-cn/missing-notes-and-solutions">Source code</a>.</p>
<p>Licensed under CC BY-NC-SA.</p>
<p>See <a href="/license">here</a> for contribution &amp; translation guidelines.</p>
</div>
