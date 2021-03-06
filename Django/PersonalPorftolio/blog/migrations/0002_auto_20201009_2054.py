# Generated by Django 3.1.2 on 2020-10-10 01:54

from django.db import migrations, models
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('blog', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='blog',
            name='body',
            field=models.TextField(default=''),
        ),
        migrations.AddField(
            model_name='blog',
            name='image',
            field=models.ImageField(default='', upload_to='blogposts/'),
        ),
        migrations.AddField(
            model_name='blog',
            name='pub_date',
            field=models.DateField(default=django.utils.timezone.now),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='blog',
            name='title',
            field=models.CharField(default='TitleHere', max_length=50),
            preserve_default=False,
        ),
    ]
